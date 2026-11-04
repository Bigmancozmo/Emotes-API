#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteFrameCache.hpp>
#include "../Utils.hpp"

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