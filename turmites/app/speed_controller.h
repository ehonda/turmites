#pragma once

#include <memory>

#include "SDL.h"

namespace turmites_sim::app {

// TODO: Rework, better initialization, think about responsibilities!
//
// This class is responsible for controlling rendering step and speed
//		-> well designed?
//		-> better name: application_speed_controller
class SpeedController {
public:
	void setRenderer(const std::shared_ptr<SDL_Renderer> renderer);
	
	void render(long framecount);

	void handleEvent(SDL_Event& e);

private:
	void faster();
	void slower();
	void printState();

	enum class Mode : int {
		RENDER_EVERY_NTH = 0,	// Better name for this mode?
		DELAY
	};

	const int MIN_RENDER_STEP = 1;
	const int MAX_RENDER_STEP = 1 << 20;

	const int MIN_DELAY = 1;
	const int MAX_DELAY = 1 << 8;

	Mode mode_ = Mode::RENDER_EVERY_NTH;
	int renderStep_ = MIN_RENDER_STEP;
	int delay_ = MIN_DELAY;

	std::shared_ptr<SDL_Renderer> renderer_;
};

}