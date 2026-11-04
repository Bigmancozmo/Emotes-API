#include "Util.hpp"
#include "Util.hpp"
#include "Util.hpp"

Utils::findRegisteredEmote(std::string name) {
	auto it = std::find_if(Shared::registeredEmotes.begin(), Shared::registeredEmotes.end(), [&] (auto const& entry) { return entry.first == name; });
	if (it != Shared::registeredEmotes.end())
		return it->second;
	return nullptr;
}

CCNode* Utils::getLowestChildNode(CCNode* parent, bool ignoreAdditionalBtns) {
	auto children = parent->getChildren();
	if (children == nullptr) return nullptr;

	float lowestY = 0;
	CCNode* lowestYNode = nullptr;
	for (int i = 0; i < children->count(); i++) {
		auto child = static_cast<CCNode*>(children->objectAtIndex(i));
		if (!child || (child->getTag() == 0xEA121 && ignoreAdditionalBtns)) continue;
		if (child->getPositionY() < lowestY) {
			lowestYNode = child;
			lowestY = child->getPositionY();
		}
	}

	return lowestYNode;
}

CCNode* Utils::getLeftestChildNode(CCNode* parent, bool ignoreAdditionalBtns) {
	auto children = parent->getChildren();
	if (children == nullptr) return nullptr;

	float leftestX = 9999999;
	CCNode* leftestXNode = nullptr;
	for (int i = 0; i < children->count(); i++) {
		auto child = static_cast<CCNode*>(children->objectAtIndex(i));
		if (!child || (child->getTag() == 0xEA121 && ignoreAdditionalBtns)) continue;
		float xPos = child->getPositionX();
		if (xPos < leftestX && xPos > 0.0f) { // the close button is at 0.0f so we kinda just do this ignore it
			leftestXNode = child;
			leftestX = child->getPositionX();
		}
	}

	return leftestXNode;
}
