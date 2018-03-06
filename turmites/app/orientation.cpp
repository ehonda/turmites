#include "orientation.h"

#include <stdexcept>

namespace turmites_sim::orientation {

namespace {

constexpr int TICKS_ON_COMPASS = 4;

Orientation stepOnCompass(Orientation orientation, int ticks) {
	int newOrientation = (static_cast<int>(orientation) + ticks) % TICKS_ON_COMPASS;
	return static_cast<Orientation>(newOrientation);
}

}

Orientation rotateOrientation(Orientation orientation, Turn turn) {
	switch (turn) {
	case Turn::NO_TURN:
		return orientation;
	case Turn::RIGHT_90:
		return stepOnCompass(orientation, 1);
	case Turn::LEFT_90:
		return stepOnCompass(orientation, 3);
	case Turn::U_TURN:
		return stepOnCompass(orientation, 2);
	default:
		throw std::invalid_argument("Invalid Turn passed to orientation::rotateOrientation");
	}
}

}
