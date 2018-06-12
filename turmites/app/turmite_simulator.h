#pragma once

#include <vector>

#include "grid.h"
#include "turmite.h"

namespace turmites_sim {

class TurmiteSimulator {
public:
	void stepTurmites();

	void addTurmite(const turmite::Turmite& turmite);
	void centerTurmites() noexcept;
	const std::vector<turmite::Turmite>& getTurmites() const noexcept;
	std::vector<turmite::Turmite>& getTurmites() noexcept;

	void setGridSize(std::size_t n);
	const grid::Grid& getGrid() const noexcept;

private:
	grid::Grid grid_;
	std::vector<turmite::Turmite> turmites_;
};

}
