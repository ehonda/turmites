#include "testing_utils.h"

#include "gtest/gtest.h"

void turmites_sim_test::expectGridCellStateAt(
	const turmites_sim::grid::Grid& grid, 
	const turmites_sim::grid::Position& pos,
	turmites_sim::grid::CellState expectedState)
{
	const auto& actualState = grid.getCellStateAt(pos);
	EXPECT_EQ(actualState, expectedState);
}

void turmites_sim_test::expectTurmitePositionOnGrid(
	const turmites_sim::grid::Grid& grid, 
	const turmites_sim::turmite::Turmite& turmite, 
	const turmites_sim::grid::Position & expectedPos)
{
	const auto& actualPos = turmite.getPosition();
	EXPECT_EQ(actualPos, expectedPos);
}
