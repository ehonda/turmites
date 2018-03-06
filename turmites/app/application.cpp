#include "application.h"

#include "sdl2_utils.h"

namespace turmites_sim::app {

void Application::run() {
	initialize();
	while (running_) {
		handleEvents();
		update();
		render();
		SDL_Delay(1);
	}
	cleanup();
}

void Application::initialize() {
	initializeSDL();
	initializeController();
	running_ = true;
}

void Application::initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		sdl2_utils::logError("SDL_Init");
		return;
	}

	window_ = std::shared_ptr<SDL_Window>(
		SDL_CreateWindow("Turmites sim", 100, 100, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
		sdl2_utils::getWindowDeleter());
	if (!window_) {
		sdl2_utils::logError("SDL_CreateWindow");
		return;
	}

	renderer_ = std::shared_ptr<SDL_Renderer>(
		SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
		sdl2_utils::getRendererDeleter());
	if (!renderer_) {
		sdl2_utils::logError("SDL_CreateRenderer");
		return;
	}
}

void Application::initializeController() {
	controller_.setWindow(window_);
	controller_.setRenderer(renderer_);

	const std::size_t N = 100;
	TurmiteSimulator sim;
	sim.setGridSize(N);

	//turmite::Turmite turmite(turmite::getLangtonsAntTransitionTable());
	turmite::Turmite turmite(turmite::getRandomClassicalTransitionTable(5));
	turmite.setPosition({ N / 2, N / 2 });
	sim.addTurmite(turmite);

	controller_.setSimulator(sim);
}

void Application::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			running_ = false;
	}
}

void Application::update() {
	controller_.update();
}

void Application::render() {
	SDL_RenderPresent(renderer_.get());
}

void Application::cleanup() {
	window_.reset();
	renderer_.reset();
}

}
