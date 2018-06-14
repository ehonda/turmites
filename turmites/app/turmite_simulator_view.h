#pragma once

#include <map>
#include <memory>

#include "boost/signals2/connection.hpp"
#include "SDL.h"

#include "turmite_simulator.h"

namespace turmites_sim::mvc {

using CellStateToColorMap = std::map<grid::CellState, SDL_Color>;
CellStateToColorMap getDefaultColorMap();

class TurmiteSimulatorView {
public:
	TurmiteSimulatorView();
	~TurmiteSimulatorView();

	// CONST ARGUMENTS?
	void render(grid::Position pos, grid::CellState cell);

	void setRenderer(const std::shared_ptr<SDL_Renderer>& renderer);
	void setTurmiteSimulator(const std::shared_ptr<TurmiteSimulator>& sim);

private:
	void renderInitialGrid();
	void renderCellAt(const grid::Position& pos, grid::CellState cell);

	std::shared_ptr<SDL_Renderer> renderer_;
	CellStateToColorMap colorMap_;
	std::shared_ptr<TurmiteSimulator> simulator_;
	boost::signals2::connection gridConnection_;
};

}
