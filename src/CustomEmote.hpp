#pragma once;
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class CustomEmote {
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