#include <Geode/Geode.hpp>
#include <Geode/modify/EmotePickerPopup.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/EmoteSprite.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCSpriteFrameCache.hpp>
#include <vector>
#include <cctype>
#include <Geode/utils/general.hpp>

#include "Util.hpp"
#include "classes/CustomEmote.hpp"

using namespace geode::prelude;

class CustomEmoteRegistry {
public:
	static CustomEmoteRegistry* create(std::string name) {
		auto me = new CustomEmoteRegistry();
		for (char& c : name)
			c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		me->m_name = name;
		return me;
	}

	void addEmote(CustomEmote* emote) {
		if (finalized) {
			log::error("Tried to add an emote to registry '{}' after finalizing", m_name);
			return;
		}
		uncommitted.push_back(emote);
	}

	void finalize() {
		if (finalized) {
			log::error("Tried to finalize registry '{}' after already finalizing", m_name);
			return;
		}

		finalized = true;
		size_t registered = 0;
		for (auto emote : uncommitted) {
			if (registerEmote(emote))
				registered++;
		}
		log::info("Registered emote namespace '{}' with {}/{} emotes!", m_name, registered, uncommitted.size());
		uncommitted.clear();
	}
private:
	std::string m_name;
	std::vector<CustomEmote*> uncommitted;
	bool finalized;

	bool registerEmote(CustomEmote* emote) {
		auto name = m_name + "_" + emote->m_name;

		if (name.size() > 16) {
			if (!Shared::anyEmotesFailed)
				Notification::create(std::string("Some emotes were not added. More info is in the console."), NotificationIcon::Error, 4.0f)->show();
			Shared::anyEmotesFailed = true;

			bool namespaceTooLong = m_name.size() > 5;
			bool nameTooLong = emote->m_name.size() > 10;

			if (namespaceTooLong)
				log::error("Couldn't add emote ':{}:' - Namespace name '{}' must be 5 or less characters long", name, m_name);
			if (nameTooLong)
				log::error("Couldn't add emote ':{}:' - Emote name '{}' must be 10 or less characters long", name, emote->m_name);

			return false;
		}

		auto existing = Utils::findRegisteredEmote(name);

		if (existing) {
			Notification::create(std::string("Duplicated emote '" + name + "'"), NotificationIcon::Warning)->show();
			log::warn("Tried to add emote '{},' but it already exists. The old emote will *not* be replaced by the new one.", name);
		} else {
			// put the emote in all 3 registries
			// why are there 3? i dont know it just happened to be that way
			emote->m_index = Shared::registeredEmoteIDs.size();
			emote->m_namespacedName = name;

			Shared::registeredEmotes.emplace_back(name, emote);
			Shared::registeredEmoteIDs.push_back(emote);

			auto group = std::find_if(Shared::displayGroups.begin(), Shared::displayGroups.end(), [&] (auto const& entry) {
				return entry.first == m_name;
			});

			if (group == Shared::displayGroups.end()) {
				Shared::displayGroups.emplace_back(m_name, std::vector<CustomEmote*>{});
				group = std::prev(Shared::displayGroups.end());
			}

			group->second.push_back(emote);
		}

		return true;
	}
};

// weird implementation cause createWithDef wouldn't call but it works so who cares
class $modify(CCSpriteFrameCache) {
	CCSpriteFrame* spriteFrameByName(const char* name) {
		auto str = std::string_view(name);
		// this is a mess but it works so
		if (str.size() >= 22) {
			if (str.starts_with("particle_") && str.ends_with("_001.png")) {
				try {
					auto trimmed = std::string_view(str.substr(9, str.size() - 17));
					auto num = numFromString<int>(trimmed);

					if (num) {
						auto value = num.unwrap();
						if (value <= -5000) {
							auto customEmoteId = -5000 - value;
							if (customEmoteId >= 0 && customEmoteId < Shared::registeredEmoteIDs.size()) {
								auto customEmote = Shared::registeredEmoteIDs[customEmoteId];
								if (customEmote) {
									return CCSpriteFrameCache::spriteFrameByName(customEmote->m_texture.c_str());
								}
							}
						}
					}
				} catch (...) {}
			}
		}
		return CCSpriteFrameCache::spriteFrameByName(name);
	}
};

class $modify(EmoteSprite) {
	static EmoteDef* defForName(std::string const& name) {
		auto custom = Utils::findRegisteredEmote(name);
		// we can't use m_namespacedName here because we dont have access to it
		if (custom) return custom->getDef();
		return EmoteSprite::defForName(name);
	}
};

std::unordered_map<CCNode*, int> emoteButtonGroups;
class $modify(EmotePickerPopupMod, EmotePickerPopup) {
	void layoutAdditionalEmotes() {
		float cellSize = 26.0f;

		CCNode* lowestNode = Utils::getLowestChildNode(this->m_scrollContentMenu);
		CCNode* leftestNode = Utils::getLeftestChildNode(this->m_scrollContentMenu);
		if (lowestNode == nullptr) return;
		if (leftestNode == nullptr) return;

		float leftestX = leftestNode->getPositionX();
		float lowestY = lowestNode->getPositionY();
		auto children = this->m_scrollContentMenu->getChildren();

		std::unordered_map<int, int> idInGroups;

		for (int i = 0; i < children->count(); i++) {
			auto btn = static_cast<CCNode*>(children->objectAtIndex(i));
			if (!btn || btn->getTag() != 0xEA121) continue;

			auto it = emoteButtonGroups.find(btn);
			if (it == emoteButtonGroups.end()) continue;
			int groupIndex = it->second;

			auto idInGroup = idInGroups.find(groupIndex);
			if (idInGroup == idInGroups.end()) idInGroups[groupIndex] = -1;
			idInGroups[groupIndex] += 1;
			
			float x = leftestX + idInGroups[groupIndex] * cellSize;

			if (x > this->m_windowWidth - cellSize / 2.0f) {
				lowestY -= cellSize;
				x = leftestX;
				idInGroups[groupIndex] = 0;
			}

			float y = lowestY - (groupIndex * cellSize * 1.5);

			btn->setPosition(x, y);
		}

		// find the new lowest one and set the content height (including the custom buttons)
		CCNode* newLowest = Utils::getLowestChildNode(this->m_scrollContentMenu, false);
		if (newLowest == nullptr) return;
		this->m_scrollContentLayer->setContentHeight(5.0f - newLowest->getPositionY() + (cellSize / 2.0f));
	}

	void layoutEmoteGrid() {
		EmotePickerPopup::layoutEmoteGrid();
		if (this->m_scrollContentMenu == nullptr) return;
		log::info("Layout");
		layoutAdditionalEmotes();
	}

	void onWindowResized() {
		EmotePickerPopup::onWindowResized();
		if (this->m_scrollContentMenu == nullptr) return;
		log::info("Resized");
		layoutAdditionalEmotes();
	}

	void onBuildWindowContent() {
		EmotePickerPopup::onBuildWindowContent();

		// Normally it's just 26.0f but we want the images to appear a little smaller
		float cellSize = 26.0f * 0.75f;

		int groupIdx = 0;
		for (auto const& [name, emotes] : Shared::displayGroups) {
			groupIdx++;
			for (auto emote : emotes) {
				log::info("namespace: {}, emote: {}", name, emote->m_name);
				auto spr = CCSprite::createWithSpriteFrameName(emote->m_texture.c_str());
				spr->setScale(std::min(
					cellSize / spr->getContentSize().width,
					cellSize / spr->getContentSize().height
				));
				auto btn = CCMenuItemExt::createSpriteExtra(spr, [this, emote] (CCMenuItemSpriteExtra*) {
					this->m_input->insertAtCursor(":" + emote->getNamespacedName() + ":");
				});
				auto lowest = Utils::getLowestChildNode(m_scrollContentMenu);
				btn->setTag(0xEA121); // some random slop number that probably wont overlap with another mod
				emoteButtonGroups[btn] = groupIdx;
				this->m_scrollContentMenu->addChild(btn);
			}
		}

		//this->layoutEmoteGrid(); // Calling the original function calls this already
		layoutAdditionalEmotes();
	}

	// clean up the emoteButtonGroups
	void onClose(CCObject* sender) {
		if (this->m_scrollContentMenu) {
			auto children = this->m_scrollContentMenu->getChildren();

			for (int i = 0; i < children->count(); i++) {
				auto btn = static_cast<CCNode*>(children->objectAtIndex(i));
				if (!btn || btn->getTag() != 0xEA121) continue;
				emoteButtonGroups.erase(btn);
			}
		}

		EmotePickerPopup::onClose(sender);
	}
};

$on_mod(Loaded) {
	auto registry = CustomEmoteRegistry::create("e_api");
	registry->addEmote(CustomEmote::create("smiley", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley2", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley3", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley4", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley5", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley6", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley7", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley8", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley9", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley10", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley11", "smiley.png"_spr));
	registry->addEmote(CustomEmote::create("smiley12", "smiley.png"_spr));
	registry->finalize();

	auto registry2 = CustomEmoteRegistry::create("e_api2");
	registry2->addEmote(CustomEmote::create("ben", "benalldoh.png"_spr));
	registry2->addEmote(CustomEmote::create("ben2", "benalldoh.png"_spr));
	registry2->addEmote(CustomEmote::create("ben3", "benalldoh.png"_spr));
	registry2->addEmote(CustomEmote::create("ben4", "benalldoh.png"_spr));
	registry2->finalize();
}
