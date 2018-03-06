#pragma once

#include <vector>

#include "cell_state.h"
#include "grid.h"
#include "orientation.h"

namespace turmites_sim::turmite {

class StateTransitionTable {
public:
	// Internal types
	// ---------------------------------------------------
	using State = std::size_t;

	// For table structure see
	// https://en.wikipedia.org/wiki/Turmite#Specification
	struct TableCell {
		grid::CellState cellStateToWrite;
		orientation::Turn turn;
		State nextState;
	};
	// ---------------------------------------------------

	StateTransitionTable() = default;
	StateTransitionTable(State internalStates,
		grid::CellState cellStates);

	void setTableCell(State state, grid::CellState cellState,
		const TableCell& tableCell);

	const TableCell& getTableCell(State state, 
		grid::CellState cellState) const;

private:
	using TableRow = std::vector<TableCell>;
	std::vector<TableRow> table_;
};

StateTransitionTable getLangtonsAntTransitionTable();
StateTransitionTable getRandomClassicalTransitionTable(std::size_t cellStates);

}
