#pragma once

#include <memory>

#include "SDL.h"

#include "turmite_simulator_controller.h"

namespace turmites_sim::app {

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

class Application {
public:
	void run();

private:
	void initialize();
	void initializeSDL();
	void initializeController();

	void handleEvents();
	void update();
	void render();
	void cleanup();

	// SDL Window and renderer
	std::shared_ptr<SDL_Window> window_;
	std::shared_ptr<SDL_Renderer> renderer_;

	mvc::TurmiteSimulatorController controller_;

	bool running_ = false;
};

}
