#include "gtest/gtest.h"

#include "testing_utils.h"

#include "turmite_simulator.h"

using namespace turmites_sim;
using namespace grid;
using namespace turmite;

namespace turmites_sim_test {

class TurmiteSimulatorTest : public testing::Test {

};

TEST_F(TurmiteSimulatorTest, step_twice_with_langtons_ant) {
	TurmiteSimulator sim;
	sim.setGridSize(3);
	sim.addTurmite(getLangtonsAntTransitionTable());

	const auto& grid = sim.getGrid();
	auto& turmite = sim.getTurmites()[0];

	const Position start = { 1, 1 };
	turmite.setPosition(start);
	expectGridCellStateAt(grid, start, CELL_ZERO);
	expectTurmitePositionOnGrid(grid, turmite, start);
	// Langtons Ant: RL
	// 0 0 0	0 0 0
	// 0 ^ 0 -> 0 1 >
	// 0 0 0    0 0 0
	sim.stepTurmites();
	expectGridCellStateAt(grid, start, CELL_ONE);

	const auto afterOneStep = Position(start.x, start.y + 1);
	expectGridCellStateAt(grid, afterOneStep, CELL_ZERO);
	expectTurmitePositionOnGrid(grid, turmite, afterOneStep);

	// Langtons Ant: RL
	// 0 0 0	0 0 0
	// 0 1 > -> 0 1 1
	// 0 0 0    0 0 v
	sim.stepTurmites();
	expectGridCellStateAt(grid, afterOneStep, CELL_ONE);

	const auto afterTwoSteps = Position(afterOneStep.x + 1, afterOneStep.y);
	expectTurmitePositionOnGrid(grid, turmite, afterTwoSteps);

}

TEST_F(TurmiteSimulatorTest, center_turmites) {
	TurmiteSimulator sim;
	
	const int SIZE = 11;
	sim.setGridSize(SIZE);

	sim.addTurmite(getLangtonsAntTransitionTable());
	sim.addTurmite(getLangtonsAntTransitionTable());

	sim.centerTurmites();

	const auto& turmites = sim.getTurmites();
	const Position EXPECTED_POSITION = { SIZE / 2, SIZE / 2 };
	ASSERT_EQ(turmites.size(), 2);
	EXPECT_EQ(turmites[0].getPosition(), EXPECTED_POSITION);
	EXPECT_EQ(turmites[1].getPosition(), EXPECTED_POSITION);
}

}
