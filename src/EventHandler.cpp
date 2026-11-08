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

	auto createRegistryListener = CreateRegistry().listen(
		[&eventRegistries] (std::string name) {
			auto registry = CustomEmoteRegistry::create(name);
			eventRegistries[name] = registry;
			return ListenerResult::Propagate;
		}
	);
	createRegistryListener.leak();

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
	finalizeRegistryListener.leak();

	auto addEmoteListener = AddEmote().listen(
		[&eventRegistries, &eventEmotes] (std::string registry, std::string name, std::string texture) {
			auto it1 = eventRegistries.find(name);

			if (it1 != eventRegistries.end()) {
				auto registry = it1->second;

			} else {
				log::error("[Event System] Tried to finalize emote '{}' nonexistent registry '{}'", name, registry);
			}

			return ListenerResult::Propagate;
		}
	);
	addEmoteListener.leak();

	// Test code
	CreateRegistry().send("event");
	AddEmote().send("event", "e1", "e.png"_spr);
	FinalizeRegistry().send("event");
	FinalizeRegistry().send("gfdvs"); // logs an error
}