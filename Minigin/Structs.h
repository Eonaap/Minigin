#pragma once
#include "MiniginPCH.h"
#include <SDL.h>

namespace kaas 
{
	class Command;

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

	struct Tile
	{
		glm::vec2 pos;
		TileStates tileState = TileStates{ TileStates::standard };
	};

	struct Disc
	{
		glm::vec2 pos;
		bool InUse;
		int tileConnectionID;
		int level = -1;
	};

	struct Rect 
	{
		int x;
		int y;
		int w;
		int h;
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
		RightThumbStick
	};

	enum class PressingState
	{
		buttonPressed,
		buttonUp,
		buttonDown,
		ThumbStick
	};

	struct ControllerAction
	{
		ControllerButton button{};
		SDL_KeyCode keyboardButton{};
		Command* command{};
		PressingState state{};
		bool isDown{};
	};
}