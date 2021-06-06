#pragma once
#include "MiniginPCH.h"

namespace kaas 
{
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
}