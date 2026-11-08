# Emotes API
A Geode mod that adds an API for RobTop's comment emotes introduced in 2.209.<br/><br/>
## Example usage
Because of how RobTop implemented his parser, emote names **cannot** be longer than 16 characters. The mod uses "registries" to group emotes, in an effort to prevent conflicts. The mod will not *crash* upon a conflict, but some emotes will be unavailable (emotes added first are prioritized and not replaced).<br/>
Internally, emotes get named `{namespace}_{name}`. To prevent reaching the 16 character limit, namespaces must be **5 characters or shorter**, and emote names must be **10 characters or shorter**.<br/>
Emote images **must** be in a spritesheet for technical reasons.<br/>
[As a required dependency](https://github.com/Bigmancozmo/Emotes-API/blob/master/examples/required.md)<br/>
[As an optional dependency](https://github.com/Bigmancozmo/Emotes-API/blob/master/examples/optional.md)
## Notes
- MacOS and iOS are not supported.
	- No bindings for `EmoteSprite::defForName`
	- No bindings for `EmotePickerPopup::layoutEmoteGrid`
	- No platform-specific pads for `EmoteDef`'s members (this only makes flipX unusable, but still)
	- Maybe more, but I already disabled them from compiling so I wouldn't know
	- If these change - open a PR or an issue!