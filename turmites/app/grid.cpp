#include "grid.h"

#include <stdexcept>

namespace turmites_sim::grid {

Grid::Grid(std::size_t size)
	: size_(size), grid_(size, std::vector<CellState>(size, CELL_ZERO)) {
}

Position Grid::getNextPositionInOrientationFrom(const Position& pos,
	orientation::Orientation orientation) 
{
	using namespace orientation;
	//NOCHMAL ANSCHAUNE?!
	switch (orientation) 
	{
	case(Orientation::NORTH):
		return Position((pos.x - 1 + size_) % size_, pos.y);
	case(Orientation::EAST):
		return Position(pos.x, (pos.y + 1) % size_);
	case(Orientation::SOUTH):
		return Position((pos.x + 1) % size_, pos.y);
	case(Orientation::WEST):
		return Position(pos.x, (pos.y - 1 + size_) % size_);
	default:
		throw std::invalid_argument(
			"Missing case for orientation (Grid::getNextPositionInOrientationFrom)");
	}
}

const CellState& Grid::getCellStateAt(const Position& pos) const {
	if (pos.x >= size_ || pos.y >= size_)
		throw std::invalid_argument("The requested Position " + toString(pos) + " is out of bounds");

	return grid_[pos.y][pos.x];
}

void Grid::setCellStateAt(const Position& pos, CellState state) {
	if (pos.x >= size_ || pos.y >= size_)
		throw std::invalid_argument("The requested Position " + toString(pos) + " is out of bounds");

	grid_[pos.y][pos.x] = state;
}

const std::vector<std::vector<CellState>>& Grid::getCells() const noexcept {
	return grid_;
}

std::size_t Grid::size() const noexcept{
	return grid_.size();
}

Position::Position(std::size_t x, std::size_t y)
	: x(x), y(y)
{
}

bool Position::operator==(const Position& other) const noexcept {
	return this->x == other.x && this->y == other.y;
}

std::string toString(const Position& pos) {
	return "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
}

}
