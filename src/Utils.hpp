#pragma once

#include <Geode/Geode.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include "../include/API.hpp"

using namespace geode::prelude;

class Shared {
public:
	inline static std::vector<std::pair<std::string, CustomEmote*>> registeredEmotes;
	inline static std::vector<CustomEmote*> registeredEmoteIDs;
	inline static std::vector<std::pair<std::string, std::vector<CustomEmote*>>> displayGroups;
	inline static std::unordered_map<std::string, std::string> shortToDisplayNameMap;
	inline static bool anyEmotesFailed = false;
};

class Utils {
public:
	static CustomEmote* findRegisteredEmote(std::string name);
	static CCNode* getLowestChildNode(CCNode* parent, bool ignoreAdditionalBtns = true);
	static CCNode* getLeftestChildNode(CCNode* parent, bool ignoreAdditionalBtns = true);
	static const std::string getModPrefix();
};
