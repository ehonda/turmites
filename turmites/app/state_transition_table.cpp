#include "state_transition_table.h"

#include <chrono>
#include <random>

namespace turmites_sim::turmite {

StateTransitionTable::StateTransitionTable(State internalStates,
	grid::CellState cellStates)
	: table_(internalStates, TableRow(cellStates))
{
}

void StateTransitionTable::setTableCell(State state,
	grid::CellState cellState, const TableCell& tableCell)
{
	// THROW IF OUT OF BOUNDS

	table_[state][cellState] = tableCell;
}

const StateTransitionTable::TableCell& StateTransitionTable::getTableCell(
	State state, grid::CellState cellState) const
{
	// THROW IF OUT OF BOUNDS

	return table_[state][cellState];
}

StateTransitionTable getLangtonsAntTransitionTable()
{
	StateTransitionTable table(1, 2);
	table.setTableCell(0, 0, { 1, orientation::Turn::RIGHT_90, 0 });
	table.setTableCell(0, 1, { 0, orientation::Turn::LEFT_90, 0 });
	return table;
}

namespace {

// REFACTOR TO TURN HEADER BECAUSE OF IMPLEMENTATION DETAILS
orientation::Turn getRandomTurn() {
	const auto seed = static_cast<unsigned>(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::mt19937 gen(seed);
	std::uniform_int_distribution<int> dist(0, 3);
	return static_cast<orientation::Turn>(dist(gen));
}

std::vector<int> getRandomPermutationOfSize(int n) {
	const auto seed = static_cast<unsigned>(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::mt19937 gen(seed);

	std::vector<int> permutation;
	for (int i = 0; i < n; ++i)
		permutation.push_back(i);

	std::shuffle(permutation.begin(), permutation.end(), gen);
	return permutation;
}

}

StateTransitionTable getRandomClassicalTransitionTable(std::size_t cellStates) {
	StateTransitionTable table(1, cellStates);
	const auto permutation = getRandomPermutationOfSize(cellStates);
	for (std::size_t i = 0; i < cellStates; ++i)
		table.setTableCell(0, i, { permutation[i], getRandomTurn(), 0 });
	return table;
}

}
