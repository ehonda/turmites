#include "state_transition_table.h"

#include <chrono>
#include <random>

#include "cereal/archives/xml.hpp"
#include "cereal/types/vector.hpp"

namespace turmites_sim::turmite {

// ----------------------------------------------------------------------------------
// CLASS METHODS

template<class Archive>
void StateTransitionTable::TableCell::serialize(Archive& archive) {
	archive(cereal::make_nvp("write_color", cellStateToWrite),
		cereal::make_nvp("turn", turn),
		cereal::make_nvp("next_state", nextState));
}

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

grid::CellState StateTransitionTable::getNumberOfCellStates() const noexcept {
	if (table_.empty())
		return 0;
	else
		return table_[0].size();
}

StateTransitionTable::State 
StateTransitionTable::getNumberOfInternalStates() const noexcept {
	return table_.size();
}

template<class Archive>
void StateTransitionTable::serialize(Archive& archive) {
	archive(cereal::make_nvp("transition_table", table_));
}



// ----------------------------------------------------------------------------------
// FREE FUNCTIONS

bool operator==(const StateTransitionTable::TableCell& lhs,
	const StateTransitionTable::TableCell& rhs)
{
	return lhs.cellStateToWrite == rhs.cellStateToWrite
		&& lhs.turn == rhs.turn
		&& lhs.nextState == rhs.nextState;
}

bool operator!=(const StateTransitionTable::TableCell& lhs, 
	const StateTransitionTable::TableCell& rhs)
{
	return !(lhs == rhs);
}

bool operator==(const StateTransitionTable& lhs, const StateTransitionTable& rhs) {
	return lhs.table_ == rhs.table_;
}

bool operator!=(const StateTransitionTable& lhs,
	const StateTransitionTable& rhs)
{
	return !(lhs == rhs);
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

StateTransitionTable getRandomTransitionTable(
	StateTransitionTable::State numberOfInternalStates, 
	grid::CellState numberOfCellStates)
{
	const auto seed = static_cast<unsigned>(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::mt19937 gen(seed);

	// TODO:
	//		numberOfCellStates == 0 abfangen
	//		numberOfInternalStates == 0 abfangen
	std::uniform_int_distribution<StateTransitionTable::State>
		stateDist(0, numberOfInternalStates - 1);
	std::uniform_int_distribution<grid::CellState>
		cellDist(0, numberOfCellStates - 1);

	StateTransitionTable table(numberOfInternalStates, numberOfCellStates);

	for (std::size_t i = 0; i < numberOfInternalStates; ++i)
		for (int j = 0; j < numberOfCellStates; ++j)
			table.setTableCell(i, j, { cellDist(gen), getRandomTurn(), stateDist(gen) });

	return table;
}

void saveTransitionTable(const StateTransitionTable& table, std::ostream& os) {
	cereal::XMLOutputArchive archive(os);
	archive(cereal::make_nvp("table", table));
}

StateTransitionTable loadTransitionTable(std::istream& is) {
	cereal::XMLInputArchive archive(is);

	StateTransitionTable loadedTable;
	archive(cereal::make_nvp("table", loadedTable));
	
	return loadedTable;
}

}
