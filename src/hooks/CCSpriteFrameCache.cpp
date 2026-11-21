#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteFrameCache.hpp>
#include <charconv>
#include "../Utils.hpp"

using namespace geode::prelude;

// weird implementation cause createWithDef wouldn't call but it works so who cares
class $modify(CCSpriteFrameCache) {
	CCSpriteFrame* spriteFrameByName(const char* name) {
		auto str = std::string_view(name);
		// this is a mess but it works so
		if (str.size() >= 22) {
			if (str.starts_with("particle_") && str.ends_with("_001.png")) {
				auto trimmed = str.substr(9, str.size() - 17);
				int particleIndex;

				auto [parsedEnd, parseError] = std::from_chars(
					trimmed.data(),
					trimmed.data() + trimmed.size(),
					particleIndex
				);

				if (parseError == std::errc{} && parsedEnd == trimmed.data() + trimmed.size()) {
					if (particleIndex <= -5000) {
						auto customEmoteId = -5000 - particleIndex;
						if (customEmoteId >= 0 && customEmoteId < Shared::registeredEmoteIDs.size()) {
							auto customEmote = Shared::registeredEmoteIDs[customEmoteId];
							if (customEmote) {
								auto cchar = customEmote->m_texture.c_str();
								return CCSpriteFrameCache::spriteFrameByName(cchar);
							}
						}
					}
				}
			}
		}
		return CCSpriteFrameCache::spriteFrameByName(name);
	}
};
