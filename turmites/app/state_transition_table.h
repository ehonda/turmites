#pragma once

#include <iostream>
#include <vector>

#include "cell_state.h"
#include "grid.h"
#include "orientation.h"

namespace turmites_sim::turmite {

class StateTransitionTable {
public:
	// Friends
	// ---------------------------------------------------
	friend bool operator==(const StateTransitionTable&,
		const StateTransitionTable&);

	// Internal types
	// ---------------------------------------------------
	using State = std::size_t;

	// For table structure see
	// https://en.wikipedia.org/wiki/Turmite#Specification
	struct TableCell {
		grid::CellState cellStateToWrite;
		orientation::Turn turn;
		State nextState;

		template<class Archive>
		void serialize(Archive& archive);
	};
	// ---------------------------------------------------

	StateTransitionTable() = default;
	StateTransitionTable(State internalStates,
		grid::CellState cellStates);

	void setTableCell(State state, grid::CellState cellState,
		const TableCell& tableCell);
	const TableCell& getTableCell(State state, 
		grid::CellState cellState) const;

	grid::CellState getNumberOfCellStates() const noexcept;
	State getNumberOfInternalStates() const noexcept;

	template<class Archive>
	void serialize(Archive& archive);

private:
	using TableRow = std::vector<TableCell>;
	std::vector<TableRow> table_;
};

// -----------------------------------------------------------------------
// COMPARISON FUNCTIONS

bool operator==(const StateTransitionTable::TableCell& lhs,
	const StateTransitionTable::TableCell& rhs);
bool operator!=(const StateTransitionTable::TableCell& lhs,
	const StateTransitionTable::TableCell& rhs);

bool operator==(const StateTransitionTable& lhs,
	const StateTransitionTable& rhs);
bool operator!=(const StateTransitionTable& lhs,
	const StateTransitionTable& rhs);

// -----------------------------------------------------------------------
// UTILITY FUNCTIONS

StateTransitionTable getLangtonsAntTransitionTable();
StateTransitionTable getRandomClassicalTransitionTable(std::size_t cellStates);
StateTransitionTable getRandomTransitionTable(
	StateTransitionTable::State numberOfInternalStates,
	grid::CellState numberOfCellStates);

// TODO
// - FAILURE SAFETY (NON EXISTANT PATH ETC)
void saveTransitionTable(const StateTransitionTable& table, std::ostream& os);
StateTransitionTable loadTransitionTable(std::istream& is);

}
