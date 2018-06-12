#include "turmite_simulator_controller.h"

#include <fstream>
#include <iostream>
#include <filesystem>

namespace turmites_sim::mvc {

void TurmiteSimulatorController::handleEvent(const SDL_Event& e) {
	switch (e.type) 
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym) 
		{
		case SDLK_l:
			handleLoadRequest();
			break;

		case SDLK_s:
			handleSaveRequest();
			break;

		case SDLK_r:
			handleRandomTurmiteRequest();
			break;

		case SDLK_n:
			handleNewTurmiteRequest();
			break;
		}
	}
}

void TurmiteSimulatorController::update() {
	simulator_->stepTurmites();
	view_.render();
}

const std::shared_ptr<TurmiteSimulator>& 
TurmiteSimulatorController::getSimulator() const noexcept {
	return simulator_;
}

void TurmiteSimulatorController::setWindow(const std::shared_ptr<SDL_Window>& window) {
	window_ = window;
}

void TurmiteSimulatorController::setRenderer(const std::shared_ptr<SDL_Renderer>& renderer) {
	view_.setRenderer(renderer);
}

void TurmiteSimulatorController::setSimulator(const TurmiteSimulator& simulator) {
	simulator_ = std::make_shared<TurmiteSimulator>(simulator);
	view_.setTurmiteSimulator(simulator_);
}

// TODO: BETTER ERROR HANDLING IN GENERAL
void TurmiteSimulatorController::loadTurmite(std::string_view name) {
	namespace fs = std::filesystem;
	const auto WORKING_DIRECTORY = fs::current_path();
	const auto SAVE_DIRECTORY = WORKING_DIRECTORY / SAVE_DIRECTORY_NAME;

	// Check if file exists
	if (!fs::exists(SAVE_DIRECTORY / name)) {
		std::cout << "Loading turmite failed. File"
			<< (SAVE_DIRECTORY / name)
			<<"does not exist."
			<< std::endl;
		return;
	}

	// Try to open file
	std::ifstream saveFile(SAVE_DIRECTORY / name);
	if (!saveFile.is_open()) {
		std::cout << "Loading turmite failed. Could not open "
			<< (SAVE_DIRECTORY / name) << std::endl;
		return;
	}

	// Build new simulator
	TurmiteSimulator newSimulator;
	// Restore current grid size if set
	if (simulator_)
		newSimulator.setGridSize(simulator_->getGrid().size());
	newSimulator.addTurmite(turmite::loadTransitionTable(saveFile));
	newSimulator.centerTurmites();

	setSimulator(newSimulator);
}

// TODO: BETTER ERROR HANDLING IN GENERAL
void TurmiteSimulatorController::saveTurmite(std::string_view name) const {
	namespace fs = std::filesystem;
	const auto WORKING_DIRECTORY = fs::current_path();
	const auto SAVE_DIRECTORY = WORKING_DIRECTORY / SAVE_DIRECTORY_NAME;
	
	if (!fs::exists(SAVE_DIRECTORY)) {
		std::error_code err;
		if (!fs::create_directory(SAVE_DIRECTORY, err)) {
			// TODO: BETTER ERROR HANDLING
			std::cout << "Saving turmite failed. Could not create directory "
				<< SAVE_DIRECTORY_NAME << " in current working directory: "
				<< WORKING_DIRECTORY << std::endl;

			std::cout << "Error message: " << err << std::endl;
			return;
		}
	}

	// TODO: ERROR HANDLING FOR ZERO TURMITES
	const auto& turmites = simulator_->getTurmites();
	if (turmites.empty())
		throw std::runtime_error("No turmite to be saved.");
	// Extract state transition table
	const auto& table = turmites.begin()->getStateTransitionTable();

	// Check if name already exists
	if (fs::exists(SAVE_DIRECTORY / name)) {
		std::cout << "Saving turmite failed. Savefile name already taken."
			<< std::endl;
		return;
	}

	// Create file and save turmite
	std::ofstream saveFile(SAVE_DIRECTORY / name);
	if (!saveFile.is_open()) {
		std::cout << "Saving turmite failed. Could not create save file."
			<< std::endl;
		return;
	}
	turmite::saveTransitionTable(table, saveFile);
}

std::string TurmiteSimulatorController::promptFileName() const {
	std::string fileName;
	std::cout << "Enter filename:" << std::endl;
	std::cin >> fileName;
	return fileName;
}

void TurmiteSimulatorController::handleLoadRequest() {
	std::cout << "Loading Turmite" << std::endl;
	const auto FILENAME = promptFileName();
	loadTurmite(FILENAME);
}

void TurmiteSimulatorController::handleSaveRequest() const {
	std::cout << "Saving Turmite" << std::endl;
	const auto FILENAME = promptFileName();
	saveTurmite(FILENAME);
}

void TurmiteSimulatorController::handleRandomTurmiteRequest() {
	if (!simulator_)
		throw std::runtime_error("No simulator.");

	const auto& turmites = simulator_->getTurmites();
	if (turmites.empty())
		throw std::runtime_error("No turmite on grid.");

	// Get random transition table with same properties
	const auto stateTable = turmites[0].getStateTransitionTable();
	const auto internalStates = stateTable.getNumberOfInternalStates();
	const auto cellStates = stateTable.getNumberOfCellStates();
	const auto newStateTable = turmite::getRandomTransitionTable(
		internalStates, cellStates);

	// Make new simulator
	TurmiteSimulator newSimulator;
	newSimulator.setGridSize(simulator_->getGrid().size());
	newSimulator.addTurmite(newStateTable);
	newSimulator.centerTurmites();
	setSimulator(newSimulator);
}

void TurmiteSimulatorController::handleNewTurmiteRequest() {
	int internalStates = 0;
	int cellStates = 0;
	int gridSize = 100;

	std::cout << "Enter number of internal states:" << std::endl;
	std::cin >> internalStates;

	std::cout << "Enter number of cell states:" << std::endl;
	std::cin >> cellStates;

	/*std::cout << "Enter grid size:" << std::endl;
	std::cin >> gridSize;*/

	// Make new simulator
	TurmiteSimulator newSimulator;
	newSimulator.setGridSize(gridSize);
	newSimulator.addTurmite(turmite::getRandomTransitionTable(internalStates, cellStates));
	newSimulator.centerTurmites();
	setSimulator(newSimulator);
}

}
