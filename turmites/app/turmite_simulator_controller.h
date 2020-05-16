#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "SDL.h"

#include "turmite_simulator.h"
#include "turmite_simulator_view.h"

namespace turmites_sim::mvc {

const std::string SAVE_DIRECTORY_NAME = "turmites_savefiles";

class TurmiteSimulatorController {
public:
	void handleEvent(const SDL_Event& e);
	void update();
	void renderAll();

	const std::shared_ptr<TurmiteSimulator>& getSimulator() const noexcept;

	void setWindow(const std::shared_ptr<SDL_Window>& window);
	void setRenderer(const std::shared_ptr<SDL_Renderer>& renderer);
	void setSimulator(const TurmiteSimulator& simulator);

	void loadTurmite(std::string_view name);
	void saveTurmite(std::string_view name) const;

private:
	std::string promptFileName() const;

	void handleLoadRequest();
	void handleSaveRequest() const;
	void handleRestartRequest();
	void handleRandomTurmiteRequest();
	void handleRandomRoundTripRequest();
	void handleNewTurmiteRequest();
	void handleMutationRequest();
	void handleInfoRequest();

	std::shared_ptr<SDL_Window> window_;

	std::shared_ptr<TurmiteSimulator> simulator_;
	TurmiteSimulatorView view_;
};

}
