#pragma once

#include <Geode/Geode.hpp>
#include <vector>
#include <string>
#include "classes/CustomEmote.hpp"

using namespace geode::prelude;

class Shared {
public:
	inline static std::vector<std::pair<std::string, CustomEmote*>> registeredEmotes;
	inline static std::vector<CustomEmote*> registeredEmoteIDs;
	inline static std::vector<std::pair<std::string, std::vector<CustomEmote*>>> displayGroups;
	inline static bool anyEmotesFailed = false;
};

class Utils {
public:
	static CustomEmote* findRegisteredEmote(std::string name);
	static CCNode* getLowestChildNode(CCNode* parent, bool ignoreAdditionalBtns = true);
	static CCNode* getLeftestChildNode(CCNode* parent, bool ignoreAdditionalBtns = true);
};
