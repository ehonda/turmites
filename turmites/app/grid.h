#pragma once

#include <string>
#include <vector>

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
	Grid() = default;
	Grid(std::size_t size);

	Position getNextPositionInOrientationFrom(const Position& pos,
		orientation::Orientation orientation);

	const CellState& getCellStateAt(const Position& pos) const;
	void setCellStateAt(const Position& pos, CellState state);

	const std::vector<std::vector<CellState>>& getCells() const noexcept;
	std::size_t size() const noexcept;

private:
	std::size_t size_;
	std::vector<std::vector<CellState>> grid_;
};

}
