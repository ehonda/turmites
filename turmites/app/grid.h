#pragma once

#include <string>
#include <vector>

#include "boost/signals2/signal.hpp"

#include "cell_state.h"
#include "orientation.h"

namespace turmites_sim::grid {

struct Position {
	Position() = default;
	Position(std::size_t x, std::size_t y);

	bool operator==(const Position& other) const noexcept;

	std::size_t x, y;
};

std::string toString(const Position& pos);

class Grid {
public:
	using CellUpdateHandler = boost::signals2::signal<void(Position, CellState)>;

	Grid() = default;
	Grid(std::size_t size);

	// BETTER IMPLEMENTATION! EXCEPTION SAFETY!
	Grid(const Grid& other);
	Grid& operator=(const Grid& other);

	Position getNextPositionInOrientationFrom(const Position& pos,
		orientation::Orientation orientation);

	const CellState& getCellStateAt(const Position& pos) const;
	void setCellStateAt(const Position& pos, CellState state);

	const std::vector<std::vector<CellState>>& getCells() const noexcept;
	std::size_t size() const noexcept;

	// BETTER: DONT RELY ON CALLERS TO DISCONNECT BEFORE THEY ARE DESTROYED
	// https://www.boost.org/doc/libs/1_67_0/doc/html/signals2/tutorial.html#id-1.3.37.4.6
	boost::signals2::connection subscribeToCellUpdates(
		CellUpdateHandler::slot_type handler);

private:
	CellUpdateHandler cellUpdateSignal_;
	std::size_t size_;
	std::vector<std::vector<CellState>> grid_;
};

}
