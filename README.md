# Emotes API
A Geode mod that adds an API for RobTop's comment emojis introduced in 2.209.<br/>
**if you're trying to compile it then unless [my pr](https://github.com/geode-sdk/bindings/pull/1476) has been merged you'll need [my bindings fork](https://github.com/Bigmancozmo/bindings)**<br/>
## Example usage
Because of how RobTop implemented his parser, emoji names **cannot** be longer than 16 characters. The mod uses "registries" to group emojis, in an effort to prevent conflicts. The mod will not *crash* upon a conflict, but some emojis will be unavailable (emojis added first are prioritized and not replaced).<br/>
Internally, emojis get named `{namespace}_{name}`. To prevent reaching the 16 character limit, namespaces must be **5 characters or shorter**, and emoji names must be **10 characters or shorter**.<br/>
```cpp
// You can have uppercase letters in your registry/emote names, but internally they're converted to lowercase
auto registry = CustomEmoteRegistry::create("mod1"); // Cannot be longer than 5 characters.
registry->addEmote(CustomEmote::create("smiley", "smiley.png"_spr));
registry->addEmote(CustomEmote::create("frown", "frown.png"_spr));
registry->addEmote(CustomEmote::create("1", "one.png"_spr));
registry->addEmote(CustomEmote::create("2", "two.png"_spr));
registry->addEmote(CustomEmote::create("3", "three.png"_spr));
registry->finalize(); // You must finalize it! After finalizing, you cannot add more emotes.

// One mod can have multiple registries! Each registry is displayed as its own "group" in the emoji picker.
// For example, one mod could add a registry for symbols and another for faces, and they would show separated.
auto registry2 = CustomEmoteRegistry::create("mod2");
registry2->addEmote(CustomEmote::create("robtop", "games.png"_spr));

// You can modify the emote! These functions are chainable.
registry2->addEmote(CustomEmote::create("geometry", "dash.png"_spr)->setScale(2.0f)->setXOffset(1.0f));

registry2->finalize();
```
Emote images **must** be in a spritesheet for technical reasons.<br/>
If you're lazy (like me), there's a `QUICK_EMOTE` macro. It assumes the .png and emote have the same name. Usage example:
```
auto registry = CustomEmoteRegistry::create("mymod");

// You must pass the registry and the image/emote name *WITHOUT* quotes
QUICK_EMOTE(registry, smiley);
registry->finalize();
```
The above example would expand to:
```
auto registry = CustomEmoteRegistry::create("mymod");
// Note that the .png and emote name are the same.
registry->addEmote(CustomEmote::create("smiley", "smiley.png"_spr));
QUICK_EMOTE(registry, smiley);
registry->finalize();
```