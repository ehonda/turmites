#include "turmite_simulator_view.h"

namespace turmites_sim::mvc {

TurmiteSimulatorView::TurmiteSimulatorView()
	: colorMap_(getDefaultColorMap())
{
}

void TurmiteSimulatorView::render() {
	int width, height;
	SDL_GetRendererOutputSize(renderer_.get(), &width, &height);

	const auto& grid = simulator_->getGrid();
	const auto& cells = grid.getCells();
	const auto size = cells.size();
	int rect_width = width / size;
	int rect_height = height / size;

	for (std::size_t x = 0; x < size; ++x) {
		for (std::size_t y = 0; y < size; ++y) {
			const auto& color = colorMap_.at(grid.getCellStateAt({ x, y }));
			SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b, color.a);

			int screen_x = x * rect_width;
			int screen_y = y * rect_height;
			SDL_Rect rect = { screen_x, screen_y, rect_width, rect_height };
			SDL_RenderFillRect(renderer_.get(), &rect);
		}
	}
}

void TurmiteSimulatorView::setRenderer(const std::shared_ptr<SDL_Renderer>& renderer) {
	renderer_ = renderer;
}

void TurmiteSimulatorView::setTurmiteSimulator(const std::shared_ptr<TurmiteSimulator>& sim) {
	simulator_ = sim;
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
