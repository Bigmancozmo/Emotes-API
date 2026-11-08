#include <API.hpp>

CustomEmote* CustomEmote::create(std::string name, std::string spriteName) {
	auto me = new CustomEmote();
	for (char& c : name)
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	me->m_texture = spriteName;
	me->m_name = name;
	me->m_color = ccWHITE;
	me->m_scale = 1.0f;
	me->m_xOffset = 0.0f;
	me->m_yOffset = 0.0f;
	me->m_flipX = false;
	me->m_def = nullptr;
	return me;
}

CustomEmote* CustomEmote::setColor(ccColor3B color) {
	m_color = color;
	return this;
}

CustomEmote* CustomEmote::setScale(float scale) {
	m_scale = scale;
	return this;
}

CustomEmote* CustomEmote::setXOffset(float xOffset) {
	m_xOffset = xOffset;
	return this;
}

CustomEmote* CustomEmote::setYOffset(float yOffset) {
	m_yOffset = yOffset;
	return this;
}

CustomEmote* CustomEmote::setFlipX(bool flipX) {
	m_flipX = flipX;
	return this;
}

EmoteDef* CustomEmote::getDef() {
	if (m_def) return m_def;

	auto def = new EmoteDef();
	def->m_scale = m_scale;
	def->m_particleIndex = -5000 - m_index;
	def->m_color = m_color;
	def->m_name = m_namespacedName.c_str();
	def->m_x = m_xOffset;
	def->m_y = m_yOffset;

#if !defined(GEODE_IS_IOS) && !defined(GEODE_IS_MACOS)
	def->m_flipX = m_flipX;
#endif

	this->m_def = def;
	return def;
}

std::string CustomEmote::getNamespacedName() {
	return m_namespacedName;
}