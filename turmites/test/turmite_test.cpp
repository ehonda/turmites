#include "gtest/gtest.h"

#include "testing_utils.h"

#include "grid.h"
#include "turmite.h"

using namespace turmites_sim;
using namespace grid;
using namespace turmite;

namespace turmites_sim_test {

class TurmiteTest : public testing::Test {
protected:
	void expectStateAt(const Position& pos, CellState expectedState) const {
		expectGridCellStateAt(grid_, pos, expectedState);
	}

	void expectTurmitePosition(const Position& expectedPos) const {
		expectTurmitePositionOnGrid(grid_, turmite_, expectedPos);
	}

	Turmite turmite_;
	Grid grid_;
};

TEST_F(TurmiteTest, step_twice_langtons_ant) {
	turmite_ = Turmite(getLangtonsAntTransitionTable());
	grid_ = Grid(3);

	const Position start = { 1, 1 };
	turmite_.setPosition(start);
	expectStateAt(start, CELL_ZERO);
	expectTurmitePosition(start);
	// Langtons Ant: RL
	// 0 0 0	0 0 0
	// 0 ^ 0 -> 0 1 >
	// 0 0 0    0 0 0
	turmite_.step(grid_);
	expectStateAt(start, CELL_ONE);

	const auto afterOneStep = Position(start.x, start.y + 1);
	expectStateAt(afterOneStep, CELL_ZERO);
	expectTurmitePosition(afterOneStep);

	// Langtons Ant: RL
	// 0 0 0	0 0 0
	// 0 1 > -> 0 1 1
	// 0 0 0    0 0 v
	turmite_.step(grid_);
	expectStateAt(afterOneStep, CELL_ONE);

	const auto afterTwoSteps = Position(afterOneStep.x + 1, afterOneStep.y);
	expectTurmitePosition(afterTwoSteps);
}

}
