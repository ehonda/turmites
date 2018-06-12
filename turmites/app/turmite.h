#pragma once

#include "grid.h"
#include "orientation.h"
#include "state_transition_table.h"

namespace turmites_sim::turmite {

class Turmite {
public:
	Turmite() = default;
	Turmite(StateTransitionTable table);

	void step(grid::Grid& grid);
	
	const grid::Position& getPosition() const noexcept;
	void setPosition(grid::Position pos) noexcept;

	const StateTransitionTable& getStateTransitionTable() const noexcept;

private:
	StateTransitionTable stateTransitionTable_;

	// REFACTOR STATE TO ANOTHER HEADER
	StateTransitionTable::State state_ = 0;
	orientation::Orientation orientation_ = orientation::Orientation::NORTH;
	grid::Position position_;
};

}
