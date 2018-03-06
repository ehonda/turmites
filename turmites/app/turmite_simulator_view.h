#pragma once

#include <map>
#include <memory>

#include "SDL.h"

#include "turmite_simulator.h"

namespace turmites_sim::mvc {

using CellStateToColorMap = std::map<grid::CellState, SDL_Color>;
CellStateToColorMap getDefaultColorMap();

class TurmiteSimulatorView {
public:
	TurmiteSimulatorView();

	void render();

	void setRenderer(const std::shared_ptr<SDL_Renderer>& renderer);
	void setTurmiteSimulator(const std::shared_ptr<TurmiteSimulator>& sim);

private:
	std::shared_ptr<SDL_Renderer> renderer_;
	CellStateToColorMap colorMap_;
	std::shared_ptr<TurmiteSimulator> simulator_;
};

}
