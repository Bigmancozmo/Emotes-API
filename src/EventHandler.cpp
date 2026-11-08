#include <Geode/Geode.hpp>

#include <Events.hpp>
#include <API.hpp>

#include <unordered_map>
#include <vector>
#include <string>

using namespace geode::prelude;

$on_mod(Loaded) {
	std::unordered_map<std::string, CustomEmoteRegistry*> eventRegistries;
	std::unordered_map<std::string, CustomEmote*> eventEmotes;

	//////////////////////////////////////////////////
	//////////////////// REGISTRY ////////////////////
	//////////////////////////////////////////////////

	auto createRegistryListener = CreateRegistry().listen(
		[&eventRegistries] (std::string name) {
			auto registry = CustomEmoteRegistry::create(name);
			eventRegistries[name] = registry;
			return ListenerResult::Propagate;
		}
	);

	auto finalizeRegistryListener = FinalizeRegistry().listen(
		[&eventRegistries] (std::string name) {
			auto it = eventRegistries.find(name);

			if (it != eventRegistries.end()) {
				auto registry = it->second;
				registry->finalize();
			} else {
				log::error("[Event System] Tried to finalize nonexistent registry '{}'", name);
			}

			return ListenerResult::Propagate;
		}
	);


	//////////////////////////////////////////////////
	///////////////////// EMOTE //////////////////////
	//////////////////////////////////////////////////

	auto addEmoteListener = AddEmote().listen(
		[&eventRegistries, &eventEmotes] (std::string registryName, std::string name, std::string texture) {
			auto it1 = eventRegistries.find(registryName);

			if (it1 != eventRegistries.end()) {
				// Registry exists
				auto registry = it1->second;

				auto it2 = eventEmotes.find(registryName + name);
				if (it2 != eventEmotes.end()) {
					// Emote exists in registry
					log::error("[Event System] Emote '{}' already exists in registry '{}'", name, registryName);
				} else {
					// Emote doesn't exist - add
					auto emote = CustomEmote::create(name, texture);
					registry->addEmote(emote);
					eventEmotes[registryName + name] = emote;
				}
			} else {
				// Registry doesn't exist - error
				log::error("[Event System] Tried to add emote '{}' to nonexistent registry '{}'", name, registryName);
			}

			return ListenerResult::Propagate;
		}
	);


	//////////////////////////////////////////////////
	///////////////////// CONFIG /////////////////////
	//////////////////////////////////////////////////

	auto configureEmoteScaleListener = ConfigureEmote_SetScale().listen(
		[&eventEmotes] (std::string registryName, std::string emoteName, float scale) {
			auto it = eventEmotes.find(registryName + emoteName);

			if (it != eventEmotes.end()) {
				auto emote = it->second;
				emote->setScale(scale);
			} else {
				log::error("[Event System] Tried to configure scale of nonexistent emote '{}' in registry '{}'", emoteName, registryName);
			}

			return ListenerResult::Propagate;
		}
	);

	auto configureEmoteColorListener = ConfigureEmote_SetColor().listen(
		[&eventEmotes] (std::string registryName, std::string emoteName, ccColor3B color) {
			auto it = eventEmotes.find(registryName + emoteName);

			if (it != eventEmotes.end()) {
				auto emote = it->second;
				emote->setColor(color);
			} else {
				log::error("[Event System] Tried to configure color of nonexistent emote '{}' in registry '{}'", emoteName, registryName);
			}

			return ListenerResult::Propagate;
		}
	);

	// leak all of them
	createRegistryListener.leak();
	finalizeRegistryListener.leak();

	addEmoteListener.leak();

	configureEmoteScaleListener.leak();
	configureEmoteColorListener.leak();

	// Test code
	CreateRegistry().send("event");
	AddEmote().send("event", "e1", "e.png"_spr);
	AddEmote().send("event", "v", "v.png"_spr);
	AddEmote().send("event", "e2", "e.png"_spr);
	AddEmote().send("event", "n", "n.png"_spr);
	AddEmote().send("event", "t", "t.png"_spr);
	ConfigureEmote_SetScale().send("event", "e1", 2.0f);
	ConfigureEmote_SetColor().send("event", "e1", ccBLUE);
	FinalizeRegistry().send("event");
}