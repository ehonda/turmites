#pragma once

#include "grid.h"
#include "turmite.h"

namespace turmites_sim_test {

void expectGridCellStateAt(
	const turmites_sim::grid::Grid& grid,
	const turmites_sim::grid::Position& pos,
	turmites_sim::grid::CellState expectedState);

void expectTurmitePositionOnGrid(
	const turmites_sim::grid::Grid& grid,
	const turmites_sim::turmite::Turmite& turmite,
	const turmites_sim::grid::Position& expectedPos);

}
