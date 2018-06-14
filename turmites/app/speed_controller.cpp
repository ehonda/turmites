#include "speed_controller.h"

#include <iostream>
#include <string>

namespace turmites_sim::app {

void SpeedController::setRenderer(
	const std::shared_ptr<SDL_Renderer> renderer)
{
	renderer_ = renderer;
}

void SpeedController::render(long framecount) {
	if(framecount % renderStep_ == 0)
		SDL_RenderPresent(renderer_.get());

	if (mode_ == Mode::DELAY)
		SDL_Delay(delay_);
}

void SpeedController::handleEvent(SDL_Event& e) {
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym) {
		case SDLK_KP_PLUS:
			faster();
			printState();
			break;

		case SDLK_KP_MINUS:
			slower();
			printState();
			break;
		}
	}

}

void SpeedController::faster() {
	switch (mode_) {
	case Mode::DELAY:
		if (delay_ > MIN_DELAY)
			delay_ /= 2;
		else
			mode_ = Mode::RENDER_EVERY_NTH;
		break;

	case Mode::RENDER_EVERY_NTH:
		if (renderStep_ < MAX_RENDER_STEP)
			renderStep_ *= 2;
		break;
	}
}

void SpeedController::slower() {
	switch (mode_) {
	case Mode::RENDER_EVERY_NTH:
		if (renderStep_ > MIN_RENDER_STEP)
			renderStep_ /= 2;
		else
			mode_ = Mode::DELAY;
		break;

	case Mode::DELAY:
		if (delay_ < MAX_DELAY)
			delay_ *= 2;
		break;
	}
}

void SpeedController::printState() {
	const std::string SEP = "-------------------------------------";
	const std::string MODE_STRING
		= (mode_ == Mode::RENDER_EVERY_NTH)
		? "RENDER_EVERY_NTH"
		: "DELAY";

	std::cout
		<< SEP << std::endl
		<< "SPEEDCONTROLLER STATE" << std::endl
		<< "mode  = " << MODE_STRING << std::endl
		<< "rstep = " << renderStep_ << std::endl
		<< "delay = " << delay_ << std::endl
		<< SEP << std::endl;
}

}
