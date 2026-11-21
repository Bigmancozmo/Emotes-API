#include <Geode/Geode.hpp>
#include <Geode/modify/EmoteSprite.hpp>
#include "../Utils.hpp"

using namespace geode::prelude;

class $modify(EmoteSprite) {
	// no mac
	static EmoteDef* defForName(std::string const& name) {
		auto custom = Utils::findRegisteredEmote(name);
		if (custom) return custom->getDef();
		return EmoteSprite::defForName(name);
	}
};