#include <limits>

#include "gtest/gtest.h"

#include "grid.h"

using namespace turmites_sim;
using namespace grid;
using namespace orientation;

namespace turmites_sim_test {

class GridTest : public testing::Test {
protected:
	const std::size_t GRID_SIZE = 20;

	GridTest()
		: grid_(GRID_SIZE)
	{
	}

	Grid grid_;
};

TEST_F(GridTest, get_next_position_at_border_behaves_like_on_torus) {
	const Position atBorder = { 0, 0 };
	const Orientation north = Orientation::NORTH;

	const auto& actualAdjacentPos = grid_.getNextPositionInOrientationFrom(atBorder, north);
	const Position expectedAdjacentPos = { 19, 0 };
	EXPECT_EQ(actualAdjacentPos, expectedAdjacentPos);
}

TEST_F(GridTest, get_cell_state_out_of_bounds_throws) {
	const Position outOfBoundsX(GRID_SIZE + 1, GRID_SIZE - 1);
	EXPECT_THROW(grid_.getCellStateAt(outOfBoundsX), std::invalid_argument);
	EXPECT_THROW(grid_.setCellStateAt(outOfBoundsX, CELL_ZERO), std::invalid_argument);

	const Position outOfBoundsY(GRID_SIZE - 1, GRID_SIZE + 1);
	EXPECT_THROW(grid_.getCellStateAt(outOfBoundsY), std::invalid_argument);
	EXPECT_THROW(grid_.setCellStateAt(outOfBoundsY, CELL_ZERO), std::invalid_argument);
}

TEST_F(GridTest, get_next_position_throws_for_invalid_orientation) {
	const Orientation invalidOrientation = static_cast<Orientation>(5555);
	EXPECT_THROW(grid_.getNextPositionInOrientationFrom({ 0, 0 }, invalidOrientation),
		std::invalid_argument);
}

}
