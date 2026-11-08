#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <string>

using namespace geode::prelude;

//////////////////////////////////////////////////
//////////////////// REGISTRY ////////////////////
//////////////////////////////////////////////////

class CreateRegistry : public Event<CreateRegistry, bool(std::string registryName)> {
public:
    using Event::Event;
};

class FinalizeRegistry : public Event<FinalizeRegistry, bool(std::string registryName)> {
public:
    using Event::Event;
};


//////////////////////////////////////////////////
///////////////////// EMOTE //////////////////////
//////////////////////////////////////////////////

class AddEmote : public Event<AddEmote, bool(std::string registryName, std::string emoteName, std::string spriteName)> {
public:
    using Event::Event;
};


//////////////////////////////////////////////////
///////////////////// CONFIG /////////////////////
//////////////////////////////////////////////////

class ConfigureEmote_SetScale : public Event<ConfigureEmote_SetScale, bool(std::string registryName, std::string emoteName, float scale)> {
public:
    using Event::Event;
};

class ConfigureEmote_SetColor : public Event<ConfigureEmote_SetColor, bool(std::string registryName, std::string emoteName, ccColor3B color)> {
public:
    using Event::Event;
};