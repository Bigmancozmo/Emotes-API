#include "CustomEmoteRegistry.hpp"
#include "../Util.hpp"
#include <cctype>

CustomEmoteRegistry* CustomEmoteRegistry::create(std::string name) {
	auto me = new CustomEmoteRegistry();
	for (char& c : name)
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	me->m_name = name;
	return me;
}

void CustomEmoteRegistry::addEmote(CustomEmote* emote) {
	if (finalized) {
		log::error("Tried to add an emote to registry '{}' after finalizing", m_name);
		return;
	}
	uncommitted.push_back(emote);
}

void CustomEmoteRegistry::finalize() {
	if (finalized) {
		log::error("Tried to finalize registry '{}' after already finalizing", m_name);
		return;
	}

	finalized = true;
	size_t registered = 0;
	for (auto emote : uncommitted) {
		if (registerEmote(emote))
			registered++;
	}
	log::info("Registered emote namespace '{}' with {}/{} emotes!", m_name, registered, uncommitted.size());
	uncommitted.clear();
}

bool CustomEmoteRegistry::registerEmote(CustomEmote* emote) {
	auto name = m_name + "_" + emote->m_name;

	if (name.size() > 16) {
		if (!Shared::anyEmotesFailed)
			Notification::create(std::string("Some emotes were not added. More info is in the console."), NotificationIcon::Error, 4.0f)->show();
		Shared::anyEmotesFailed = true;

		bool namespaceTooLong = m_name.size() > 5;
		bool nameTooLong = emote->m_name.size() > 10;

		if (namespaceTooLong)
			log::error("Couldn't add emote ':{}:' - Namespace name '{}' must be 5 or less characters long", name, m_name);
		if (nameTooLong)
			log::error("Couldn't add emote ':{}:' - Emote name '{}' must be 10 or less characters long", name, emote->m_name);

		return false;
	}

	auto existing = Utils::findRegisteredEmote(name);

	if (existing) {
		Notification::create(std::string("Duplicated emote '" + name + "'"), NotificationIcon::Warning)->show();
		log::warn("Tried to add emote '{},' but it already exists. The old emote will *not* be replaced by the new one.", name);
	} else {
		// put the emote in all 3 registries
		// why are there 3? i dont know it just happened to be that way
		emote->m_index = Shared::registeredEmoteIDs.size();
		emote->m_namespacedName = name;

		Shared::registeredEmotes.emplace_back(name, emote);
		Shared::registeredEmoteIDs.push_back(emote);

		auto group = std::find_if(Shared::displayGroups.begin(), Shared::displayGroups.end(), [&] (auto const& entry) {
			return entry.first == m_name;
			});

		if (group == Shared::displayGroups.end()) {
			Shared::displayGroups.emplace_back(m_name, std::vector<CustomEmote*>{});
			group = std::prev(Shared::displayGroups.end());
		}

		group->second.push_back(emote);
	}

	return true;
}
