#include <Geode/Geode.hpp>

#include "classes/CustomEmote.hpp"
#include "classes/CustomEmoteRegistry.hpp"

using namespace geode::prelude;

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
