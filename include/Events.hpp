#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <string>

using namespace geode::prelude;

//////////////////////////////////////////////////
//////////////////// REGISTRY ////////////////////
//////////////////////////////////////////////////

class CreateRegistry : public Event<CreateRegistry, bool(const char* shortName, const char* displayName)> {
public:
    using Event::Event;
};

class FinalizeRegistry : public Event<FinalizeRegistry, bool(const char* registryName)> {
public:
    using Event::Event;
};


//////////////////////////////////////////////////
///////////////////// EMOTE //////////////////////
//////////////////////////////////////////////////

class AddEmote : public Event<AddEmote, bool(const char* registryName, const char* emoteName, const char* spriteName)> {
public:
    using Event::Event;
};


//////////////////////////////////////////////////
///////////////////// CONFIG /////////////////////
//////////////////////////////////////////////////

class ConfigureEmote_SetScale : public Event<ConfigureEmote_SetScale, bool(const char* registryName, const char* emoteName, float scale)> {
public:
    using Event::Event;
};

class ConfigureEmote_SetColor : public Event<ConfigureEmote_SetColor, bool(const char* registryName, const char* emoteName, ccColor3B color)> {
public:
    using Event::Event;
};

class ConfigureEmote_SetXOffset : public Event<ConfigureEmote_SetXOffset, bool(const char* registryName, const char* emoteName, float xOffset)> {
public:
    using Event::Event;
};

class ConfigureEmote_SetYOffset : public Event<ConfigureEmote_SetYOffset, bool(const char* registryName, const char* emoteName, float yOffset)> {
public:
    using Event::Event;
};

class ConfigureEmote_SetFlipX : public Event<ConfigureEmote_SetFlipX, bool(const char* registryName, const char* emoteName, bool flipX)> {
public:
    using Event::Event;
};