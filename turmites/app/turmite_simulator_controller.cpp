#include "turmite_simulator_controller.h"

namespace turmites_sim::mvc {

void TurmiteSimulatorController::handleEvent(const SDL_Event& e) {
}

void TurmiteSimulatorController::update() {
	simulator_->stepTurmites();
	view_.render();
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

}
