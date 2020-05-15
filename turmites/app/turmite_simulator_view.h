#pragma once

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "boost/signals2/connection.hpp"
#include "SDL.h"

#include "turmite_simulator.h"

namespace turmites_sim::mvc {

//using CellStateToColorMap = std::map<grid::CellState, SDL_Color>;

using CellStateToColorMap = std::vector<SDL_Color>;
const std::string COLOR_MAP_DIRECTORY_NAME = "turmites_colormaps";

CellStateToColorMap getDefaultColorMap();
CellStateToColorMap readColorMapFile(std::string_view fileName);

class TurmiteSimulatorView {
public:
	TurmiteSimulatorView();
	~TurmiteSimulatorView();

	void handleEvent(const SDL_Event& e);

	// CONST ARGUMENTS?
	void render(grid::Position pos, grid::CellState cell);
	void renderAll();

	void setRenderer(const std::shared_ptr<SDL_Renderer>& renderer);
	void setTurmiteSimulator(const std::shared_ptr<TurmiteSimulator>& sim);

private:
	void renderCellAt(const grid::Position& pos, grid::CellState cell);

	// REFACTOR INTO INPUTHANDLER CLASS OR COLLECTION OF UTILITY FUNCTIONS
	std::string promptFileName() const;
	void handleLoadColormapRequest();
	void handleZoomRequest();

	std::shared_ptr<SDL_Renderer> renderer_;
	CellStateToColorMap colorMap_;
	std::shared_ptr<TurmiteSimulator> simulator_;
	std::shared_ptr<SDL_Texture> gridTexture_;	// TODO: Should this be unique_ptr?
	SDL_Rect viewport_;
	boost::signals2::connection gridConnection_;
};

}
