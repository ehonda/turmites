#include "gtest/gtest.h"

#include "orientation.h"

using namespace turmites_sim;
using namespace orientation;

namespace turmites_sim_test {

class OrientationTest : public testing::Test {
protected:
	void rotateByAndExpect(Turn turn, Orientation expectedOrientation) const {
		auto actualOrientation = rotateOrientation(initialOrientation_, turn);
		EXPECT_EQ(actualOrientation, expectedOrientation);
	}

	const Orientation initialOrientation_ = Orientation::NORTH;
};

TEST_F(OrientationTest, test_rotations) {
	rotateByAndExpect(Turn::NO_TURN, Orientation::NORTH);
	rotateByAndExpect(Turn::RIGHT_90, Orientation::EAST);
	rotateByAndExpect(Turn::LEFT_90, Orientation::WEST);
	rotateByAndExpect(Turn::U_TURN, Orientation::SOUTH);
}

TEST_F(OrientationTest, rotating_by_invalid_turn_throws) {
	const Turn invalidTurn = static_cast<Turn>(5555);
	EXPECT_THROW(rotateOrientation(initialOrientation_, invalidTurn), std::invalid_argument);
}

}
