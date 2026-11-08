#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <string>

using namespace geode::prelude;

class CreateRegistry : public Event<CreateRegistry, bool(std::string name)> {
public:
    using Event::Event;
};

class FinalizeRegistry : public Event<FinalizeRegistry, bool(std::string name)> {
public:
    using Event::Event;
};

class AddEmote : public Event<AddEmote, bool(std::string registry, std::string name, std::string texture)> {
public:
    using Event::Event;
};