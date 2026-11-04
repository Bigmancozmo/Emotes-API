#include <Geode/Geode.hpp>
#include <Geode/modify/EmoteSprite.hpp>
#include "../Utils.hpp"

using namespace geode::prelude;

class $modify(EmoteSprite) {
	static EmoteDef* defForName(std::string const& name) {
		auto custom = Utils::findRegisteredEmote(name);
		// we can't use m_namespacedName here because we dont have access to it
		if (custom) return custom->getDef();
		return EmoteSprite::defForName(name);
	}
};