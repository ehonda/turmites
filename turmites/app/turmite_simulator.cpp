#include "turmite_simulator.h"

namespace turmites_sim {

void TurmiteSimulator::stepTurmites() {
	for (auto& turmite : turmites_)
		turmite.step(grid_);
}

void TurmiteSimulator::addTurmite(const turmite::Turmite& turmite) {
	turmites_.push_back(turmite);
}

void TurmiteSimulator::centerTurmites() noexcept {
	const std::size_t SIZE = grid_.size();
	for (auto& turmite : turmites_)
		turmite.setPosition({ SIZE / 2, SIZE / 2 });
}

const std::vector<turmite::Turmite>& TurmiteSimulator::getTurmites() const noexcept {
	return turmites_;
}

std::vector<turmite::Turmite>& TurmiteSimulator::getTurmites() noexcept {
	return turmites_;
}

void TurmiteSimulator::setGridSize(std::size_t n) {
	grid_ = grid::Grid(n);
}

const grid::Grid& TurmiteSimulator::getGrid() const noexcept {
	return grid_;
}

grid::Grid& TurmiteSimulator::getGrid() noexcept {
	return grid_;
}

}
