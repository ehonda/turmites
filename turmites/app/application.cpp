#include "application.h"

#include "sdl2_utils.h"

namespace turmites_sim::app {

void Application::run() {
	initialize();
	while (running_) {
		handleEvents();
		update();

		////if (frames_ % 1000 == 0) {
		////	render();
		////	//SDL_Delay(1);
		////}
		//////render();
		//render();

		speedController_.render(frames_);
		
		frames_++;
	}
	cleanup();
}

void Application::initialize() {
	initializeSDL();
	initializeController();
	running_ = true;
	startingTime_ = std::chrono::steady_clock::now();
	speedController_.setRenderer(renderer_);
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

	// The flag SDL_RENDERER_PRESENTVSYNC limits the frame rate to the screens refresh rate,
	// which is not what we want here. See:
	//
	// https://stackoverflow.com/questions/21949479/is-there-a-frame-rate-cap-built-into-sdl-2-0?rq=1
	renderer_ = std::shared_ptr<SDL_Renderer>(
		SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/),
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
	turmite::Turmite turmite(turmite::getRandomClassicalTransitionTable(3));
	turmite.setPosition({ N / 2, N / 2 });
	sim.addTurmite(turmite);

	controller_.setSimulator(sim);
}

void Application::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				running_ = false;
				break;

			case SDL_KEYDOWN:
				controller_.handleEvent(e);
				speedController_.handleEvent(e);
				if (e.key.keysym.sym == SDLK_f)
					handleFPSRequest();
				if (e.key.keysym.sym == SDLK_ESCAPE)
					running_ = false;
				break;

			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
					controller_.renderAll();
			break;
		}
	}
}

void Application::update() {
	controller_.update();
}

//void Application::render() {
//	SDL_RenderPresent(renderer_.get());
//}

void Application::cleanup() {
	window_.reset();
	renderer_.reset();
}

void Application::handleFPSRequest() {
	using namespace std::chrono;
	const auto timeSinceStart = steady_clock::now() - startingTime_;

	const auto secondsSinceStart = duration_cast<seconds>(timeSinceStart);
	
	std::cout << "Avg. FPS = "
		<< (static_cast<double>(frames_) / secondsSinceStart.count())
		<< std::endl;
}

}
