#pragma once
#include "MiniginPCH.h"
#include <SDL.h>

namespace kaas 
{
	class Command;
}


//Enum classes
enum class EnemyTypes
{
	SlickSam,
	UggWrongway,
	Coily
};

enum class Event {
	LoseLife,
	ColorChange,
	Coilydefeat,
	DiscRemained,
	CatchSlickSam
};

enum class TileStates
{
	standard,
	target,
	intermediate
};

enum class MovementDirections
{
	topLeft,
	topRight,
	bottomLeft,
	bottomRight,
	UggWrongWayLeft,
	UggWrongWayRight
};

enum class TileAffection
{
	nothing,
	always,
	onlyActive
};

enum class ControllerButton
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	DPAD_LEFT,
	DPAD_RIGHT,
	DPAD_UP,
	DPAD_DOWN,
	RightThumbStick,
	LeftThumbStick
};

enum class PressingState
{
	buttonPressed,
	buttonUp,
	buttonDown,
	ThumbStick
};


//Structs
struct EnemyData
{
	EnemyTypes type = EnemyTypes::Coily;
	float spawnTime;
	int level;
};


struct Tile
{
	glm::vec2 pos{};
	TileStates tileState = TileStates{ TileStates::standard };
};

struct Disc
{
	glm::vec2 pos{};
	bool InUse = false;
	int tileConnectionID = 0;
	int level = -1;
};

struct ControllerAction
{
	ControllerButton button{};
	SDL_KeyCode keyboardButton{};
	kaas::Command* command{};
	PressingState state{};
	bool isDown{};
};
