#include "turmite_simulator_view.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

namespace turmites_sim::mvc {

TurmiteSimulatorView::TurmiteSimulatorView()
	: colorMap_(getDefaultColorMap())
{
}

TurmiteSimulatorView::~TurmiteSimulatorView() {
	gridConnection_.disconnect();
}

void TurmiteSimulatorView::handleEvent(const SDL_Event& e) {
	switch (e.type) 
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_c:
			handleLoadColormapRequest();
			break;
		}
	}
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

std::string TurmiteSimulatorView::promptFileName() const {
	std::string fileName;
	std::cout << "Enter filename:" << std::endl;
	std::cin >> fileName;
	return fileName;
}

void TurmiteSimulatorView::handleLoadColormapRequest() {
	std::cout << "Loading colormap" << std::endl;
	const auto FILENAME = promptFileName();
	colorMap_ = readColorMapFile(FILENAME);
	// Better name: renderFullGrid()
	renderInitialGrid();
}

CellStateToColorMap getDefaultColorMap() {
	CellStateToColorMap map(5, {0, 0, 0, SDL_ALPHA_OPAQUE });
	map[grid::CELL_ZERO] = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	map[grid::CELL_ONE] = { 0, 0, 0, SDL_ALPHA_OPAQUE };
	map[2] = { 255, 0, 0, SDL_ALPHA_OPAQUE };
	map[3] = { 0, 255, 0, SDL_ALPHA_OPAQUE };
	map[4] = { 0, 0, 255, SDL_ALPHA_OPAQUE };

	return map;
}

// Function used by readColorMapFile
namespace {

// TODO: BETTER ERROR HANDLING, DO NOT CHANGE ACTIVE
// COLORMAP IN CASE OF FAILURE
CellStateToColorMap parseColorMapFile(std::ifstream& file) {
	// Extract lines
	std::vector<std::string> lines;
	std::string currentLine;
	while (std::getline(file, currentLine))
		if (!currentLine.empty())
			lines.push_back(currentLine);

	// Check if lines are valid
	static const std::string COLOR_EXP_STRING = "[0-9a-fA-F]+";
	static const std::regex COLOR_EXP(COLOR_EXP_STRING);
	for (const auto& line : lines) {
		if (line.length() != 6 || !std::regex_match(line, COLOR_EXP)) {
			std::cout << "Invalid colormap file" << std::endl;
			return getDefaultColorMap();
		}
	}

	// Parse SDL_Color represented by each line
	CellStateToColorMap colorMap;
	for (const auto& line : lines) {
		SDL_Color color;
		color.r = std::stoi(line.substr(0, 2), nullptr, 16);
		color.g = std::stoi(line.substr(2, 2), nullptr, 16);
		color.b = std::stoi(line.substr(4, 2), nullptr, 16);
		color.a = SDL_ALPHA_OPAQUE;
		colorMap.push_back(color);
	}
	return colorMap;
}

}

// TODO: BETTER ERROR HANDLING, DO NOT CHANGE ACTIVE
// COLORMAP IN CASE OF FAILURE
CellStateToColorMap readColorMapFile(std::string_view fileName) {
	namespace fs = std::filesystem;
	const auto WORKING_DIRECTORY = fs::current_path();
	const auto COLOR_MAP_DIRECTORY = WORKING_DIRECTORY / COLOR_MAP_DIRECTORY_NAME;

	if (!fs::exists(COLOR_MAP_DIRECTORY)) {
		std::error_code err;
		if (!fs::create_directory(COLOR_MAP_DIRECTORY, err)) {
			std::cout << "Loading colormap failed. Standard colormap directory:"
				<< std::endl
				<< COLOR_MAP_DIRECTORY_NAME << std::endl
				<< "was not found and could not be created." << std::endl;

			std::cout << "Error message: " << err << std::endl;
			return getDefaultColorMap();
		}
		std::cout << "Creating standard colormap directory: "
			<< COLOR_MAP_DIRECTORY << std::endl;
	}

	// Check if file exists
	if (!fs::exists(COLOR_MAP_DIRECTORY / fileName)) {
		std::cout << "Loading colormap failed. File"
			<< (COLOR_MAP_DIRECTORY / fileName)
			<< " does not exist" << std::endl;
		return getDefaultColorMap();
	}

	// Try to open file
	std::ifstream colorMapFile(COLOR_MAP_DIRECTORY / fileName);
	if (!colorMapFile.is_open()) {
		std::cout << "Loading colormap failed. Could not open "
			<< (COLOR_MAP_DIRECTORY / fileName)
			<< std::endl;
		return getDefaultColorMap();
	}

	return parseColorMapFile(colorMapFile);
}

}
