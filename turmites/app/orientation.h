#pragma once

namespace turmites_sim::orientation {

enum class Orientation : int {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST
};

enum class Turn : int {
	NO_TURN = 0,
	RIGHT_90,
	LEFT_90,
	U_TURN
};

Orientation rotateOrientation(Orientation orientation, Turn turn);

}
