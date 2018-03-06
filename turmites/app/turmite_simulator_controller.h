#pragma once

#include <memory>

#include "SDL.h"

#include "turmite_simulator.h"
#include "turmite_simulator_view.h"

namespace turmites_sim::mvc {

class TurmiteSimulatorController {
public:
	void handleEvent(const SDL_Event& e);
	void update();

	void setWindow(const std::shared_ptr<SDL_Window>& window);
	void setRenderer(const std::shared_ptr<SDL_Renderer>& renderer);
	void setSimulator(const TurmiteSimulator& simulator);

private:
	std::shared_ptr<SDL_Window> window_;

	std::shared_ptr<TurmiteSimulator> simulator_;
	TurmiteSimulatorView view_;
};

}
