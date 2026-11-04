#pragma once

#include <Geode/Geode.hpp>
#include <cctype>
#include <string>
#include <vector>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
	#ifdef EMOTES_API_EXPORTING
		#define EMOTES_API_DLL __declspec(dllexport)
	#else
		#define EMOTES_API_DLL __declspec(dllimport)
	#endif
#else
	#define EMOTES_API_DLL __attribute__((visibility("default")))
#endif

class EMOTES_API_DLL CustomEmote {
public:
	static CustomEmote* create(std::string name, std::string spriteName);

	// u can chain these
	CustomEmote* setColor(ccColor3B color);
	CustomEmote* setScale(float scale);
	CustomEmote* setXOffset(float xOffset);
	CustomEmote* setYOffset(float yOffset);
	CustomEmote* setFlipX(bool flipX);

	EmoteDef* getDef();
	std::string getNamespacedName();

public:
	std::string m_texture;
	std::string m_name;
	ccColor3B m_color;
	float m_scale;
	float m_xOffset;
	float m_yOffset;
	bool m_flipX;

private:
	friend class CustomEmoteRegistry;
	int m_index;
	std::string m_namespacedName;
	EmoteDef* m_def;
};

class EMOTES_API_DLL CustomEmoteRegistry {
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