#include "turmite_simulator_view.h"

namespace turmites_sim::mvc {

TurmiteSimulatorView::TurmiteSimulatorView()
	: colorMap_(getDefaultColorMap())
{
}

TurmiteSimulatorView::~TurmiteSimulatorView() {
	gridConnection_.disconnect();
}

void TurmiteSimulatorView::render(grid::Position pos, grid::CellState cell) {
	renderCellAt(pos, cell);
	//renderInitialGrid();
}

void TurmiteSimulatorView::setRenderer(const std::shared_ptr<SDL_Renderer>& renderer) {
	renderer_ = renderer;
}

void TurmiteSimulatorView::setTurmiteSimulator(
	const std::shared_ptr<TurmiteSimulator>& sim)
{
	simulator_ = sim;
	// TODO: HOW TO HANDLE EMPTY SIMULATORS
	if (simulator_) {
		gridConnection_.disconnect();
		gridConnection_ = simulator_->getGrid().subscribeToCellUpdates(
			[this](grid::Position pos, grid::CellState cell) {
				this->render(pos, cell);
		});
		renderInitialGrid();
	}
}

void TurmiteSimulatorView::renderInitialGrid() {
	const auto& grid = simulator_->getGrid();
	const auto size = grid.size();

	for (std::size_t x = 0; x < size; ++x) {
		for (std::size_t y = 0; y < size; ++y) {
			const grid::Position pos = { x, y };
			renderCellAt(pos, grid.getCellStateAt(pos));
		}
	}
}

void TurmiteSimulatorView::renderCellAt(
	const grid::Position& pos, grid::CellState cell) 
{
	int width, height;
	SDL_GetRendererOutputSize(renderer_.get(), &width, &height);

	const auto size = simulator_->getGrid().size();
	int rect_width = width / size;
	int rect_height = height / size;

	// Draw
	const auto& color = colorMap_.at(cell);
	SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b, color.a);

	int screen_x = pos.x * rect_width;
	int screen_y = pos.y * rect_height;
	SDL_Rect rect = { screen_x, screen_y, rect_width, rect_height };
	SDL_RenderFillRect(renderer_.get(), &rect);
}

CellStateToColorMap getDefaultColorMap() {
	CellStateToColorMap map;
	map[grid::CELL_ZERO] = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	map[grid::CELL_ONE] = { 0, 0, 0, SDL_ALPHA_OPAQUE };
	map[2] = { 255, 0, 0, SDL_ALPHA_OPAQUE };
	map[3] = { 0, 255, 0, SDL_ALPHA_OPAQUE };
	map[4] = { 0, 0, 255, SDL_ALPHA_OPAQUE };

	return map;
}

}
