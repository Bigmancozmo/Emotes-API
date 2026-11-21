#include <Geode/Geode.hpp>

#include <Events.hpp>
#include <API.hpp>

#include <unordered_map>
#include <vector>
#include <string>

using namespace geode::prelude;

// i'll clean this up, eventually
$on_mod(Loaded) {
	static std::unordered_map<std::string, CustomEmoteRegistry*> eventRegistries;
	static std::unordered_map<std::string, CustomEmote*> eventEmotes;

	//////////////////////////////////////////////////
	//////////////////// REGISTRY ////////////////////
	//////////////////////////////////////////////////

	auto createRegistryListener = CreateRegistry().listen(
		[] (const char* name) {
			auto it1 = eventRegistries.find(name);

			if (it1 != eventRegistries.end()) {
				// Registry exists - error
				log::error("[Event System] Registry '{}' already exists", name);
			} else {
				// Registry doesn't exist - create
				auto registry = CustomEmoteRegistry::create(name);
				eventRegistries[name] = registry;
			}
			return ListenerResult::Propagate;
		}
	);

	auto finalizeRegistryListener = FinalizeRegistry().listen(
		[] (const char* name) {
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
		[] (const char* registryName, const char* name, const char* texture) {
			auto it1 = eventRegistries.find(registryName);

			if (it1 != eventRegistries.end()) {
				// Registry exists
				auto registry = it1->second;

				auto it2 = eventEmotes.find(std::string(registryName) + "_" + std::string(name));
				if (it2 != eventEmotes.end()) {
					// Emote exists in registry
					log::error("[Event System] Emote '{}' already exists in registry '{}'", name, registryName);
				} else {
					// Emote doesn't exist - add
					auto emote = CustomEmote::create(name, texture);
					registry->addEmote(emote);
					eventEmotes[std::string(registryName) + "_" + std::string(name)] = emote;
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
		[] (const char* registryName, const char* emoteName, float scale) {
			auto it = eventEmotes.find(std::string(registryName) + "_" + std::string(emoteName));

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
		[] (const char* registryName, const char* emoteName, ccColor3B color) {
			auto it = eventEmotes.find(std::string(registryName) + "_" + std::string(emoteName));

			if (it != eventEmotes.end()) {
				auto emote = it->second;
				emote->setColor(color);
			} else {
				log::error("[Event System] Tried to configure color of nonexistent emote '{}' in registry '{}'", emoteName, registryName);
			}

			return ListenerResult::Propagate;
		}
	);

	auto configureEmoteXOffsetListener = ConfigureEmote_SetXOffset().listen(
		[] (const char* registryName, const char* emoteName, float xOffset) {
			auto it = eventEmotes.find(std::string(registryName) + "_" + std::string(emoteName));

			if (it != eventEmotes.end()) {
				auto emote = it->second;
				emote->setXOffset(xOffset);
			} else {
				log::error("[Event System] Tried to configure X offset of nonexistent emote '{}' in registry '{}'", emoteName, registryName);
			}

			return ListenerResult::Propagate;
		}
	);

	auto configureEmoteYOffsetListener = ConfigureEmote_SetYOffset().listen(
		[] (const char* registryName, const char* emoteName, float yOffset) {
			auto it = eventEmotes.find(std::string(registryName) + "_" + std::string(emoteName));

			if (it != eventEmotes.end()) {
				auto emote = it->second;
				emote->setYOffset(yOffset);
			} else {
				log::error("[Event System] Tried to configure Y offset of nonexistent emote '{}' in registry '{}'", emoteName, registryName);
			}

			return ListenerResult::Propagate;
		}
	);

	auto configureEmoteFlipXListener = ConfigureEmote_SetFlipX().listen(
		[] (const char* registryName, const char* emoteName, bool flipX) {
			auto it = eventEmotes.find(std::string(registryName) + "_" + std::string(emoteName));

			if (it != eventEmotes.end()) {
				auto emote = it->second;
				emote->setFlipX(flipX);
			} else {
				log::error("[Event System] Tried to configure flip X of nonexistent emote '{}' in registry '{}'", emoteName, registryName);
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
	configureEmoteXOffsetListener.leak();
	configureEmoteYOffsetListener.leak();
	configureEmoteFlipXListener.leak();
}