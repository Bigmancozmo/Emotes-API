## Usage as an optional dependency
The mod can be used as an optional dependency through Geode's event system.<br/>
Please do note that the **event API may not be as complete as the regular API.**
```cpp
// Make sure you have this at the top
#include <bigmancozmo.emotes-api/Events.hpp>

// Create a registry called "event"
CreateRegistry().send("event");

// Just like in the regular API, the images must be part of a spritesheet in your mod
// The AddEmote event takes 3 paremeters: registry name, emote name, sprite name
// It is subject to the same restrictions as the regular API.
AddEmote().send("event", "e1", "e.png"_spr);
AddEmote().send("event", "v", "v.png"_spr);
AddEmote().send("event", "e2", "e.png"_spr);
AddEmote().send("event", "n", "n.png"_spr);
AddEmote().send("event", "t", "t.png"_spr);

// Finalize the registry
FinalizeRegistry().send("event");
```