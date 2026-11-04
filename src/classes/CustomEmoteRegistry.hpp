#pragma once

#include <Geode/Geode.hpp>
#include <vector>
#include <string>
#include "CustomEmote.hpp"

using namespace geode::prelude;

class CustomEmoteRegistry {
public:
	static CustomEmoteRegistry* create(std::string name);

	void addEmote(CustomEmote* emote);
	void finalize();

private:
	std::string m_name;
	std::vector<CustomEmote*> uncommitted;
	bool finalized;

	bool registerEmote(CustomEmote* emote);
};