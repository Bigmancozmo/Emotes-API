#include <Geode/Geode.hpp>
#include <Geode/modify/EmotePickerPopup.hpp>
#include "../Utils.hpp"

using namespace geode::prelude;

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
		layoutAdditionalEmotes();
	}

	void onWindowResized() {
		EmotePickerPopup::onWindowResized();
		if (this->m_scrollContentMenu == nullptr) return;
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
	void onClose(CCObject * sender) {
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