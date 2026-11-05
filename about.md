# Emotes API
Made by @Bigmancozmo
**This mod does nothing on its own. It is intended for developers to use to add custom emojis to the builtin emoji picker**
***
A mod that adds an API for RobTop's comment emojis introduced in 2.209.
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
registry2->addEmote(CustomEmote::create("geometry", "dash.png"_spr));
registry2->finalize();
```
**Further documentation is on the [README](https://github.com/Bigmancozmo/Emotes-API/blob/master/README.md)**
