#include <Geode/Geode.hpp>
#include <Geode/modify/EmotePickerPopup.hpp>
#include <Geode/modify/CCSpriteFrameCache.hpp>
#include <Geode/utils/general.hpp>

#include "Util.hpp"
#include "classes/CustomEmote.hpp"
#include "classes/CustomEmoteRegistry.hpp"

using namespace geode::prelude;

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
					// weird crash workaround
					auto textToInsert = ":" + emote->getNamespacedName() + ":";
					for (char c : textToInsert) {
						this->m_input->insertAtCursor(std::string(1, c));
					}
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
