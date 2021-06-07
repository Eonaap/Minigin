# Minigin
QBert in minigin

Most of my managers are gameObjects with a specific component.
I decided to go for this aproach, instead of actual managers that would use a singleton or allocator.

I prefer a manager to be an object, this way, not everyone can reach it, which gives security.
It also has an update and render which helped with handling the level.

My inputmanager has multiple ways of handling input. it processes its input in the game loop, but you can also just get one specific key or button.
