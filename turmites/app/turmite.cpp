#include "turmite.h"

namespace turmites_sim::turmite {

Turmite::Turmite(StateTransitionTable table)
	: stateTransitionTable_(table)
{
}

void Turmite::step(grid::Grid& grid) {
	const auto cellState = grid.getCellStateAt(position_);
	const auto& tableCell = stateTransitionTable_.getTableCell(state_, cellState);
	
	grid.setCellStateAt(position_, tableCell.cellStateToWrite);
	orientation_ = rotateOrientation(orientation_, tableCell.turn);
	
	state_ = tableCell.nextState;
	position_ = grid.getNextPositionInOrientationFrom(
		position_, orientation_);
}

const grid::Position& Turmite::getPosition() const noexcept {
	return position_;
}

void Turmite::setPosition(grid::Position pos) noexcept {
	position_ = pos;
}

const StateTransitionTable& Turmite::getStateTransitionTable() const noexcept {
	return stateTransitionTable_;
}



}
