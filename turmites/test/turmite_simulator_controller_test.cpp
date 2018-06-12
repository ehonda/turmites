#include <filesystem>

#include "gtest/gtest.h"

#include "turmite_simulator_controller.h"

using namespace turmites_sim::mvc;
using namespace turmites_sim::turmite;
namespace fs = std::filesystem;

namespace turmites_sim_test {

class TurmiteSimulatorControllerTest : public testing::Test {
protected:
	const std::size_t DEFAULT_GRID_SIZE = 100;
	const std::string DEFAULT_SAVE_NAME = "test_save_langton";
	const std::string SANDBOX_NAME = "turmites_sim_testing_sandbox";
	const fs::path INITIAL_WORKING_DIRECTORY = fs::current_path();

	// -------------------------------------------------------------------------------------
	// SANDBOX / FILESYSTEM METHODS

	void teardownSandBox() {
		const auto sandboxPath = getSandboxPath();
		if (fs::exists(sandboxPath))
			fs::remove_all(sandboxPath);
	}

	void setupSandbox() {
		teardownSandBox();
		const auto sandboxPath = getSandboxPath();
		fs::create_directory(sandboxPath);
	}

	void workInSandbox() {
		fs::current_path(getSandboxPath());
	}

	void returnToInitialWorkingDirectory() {
		fs::current_path(INITIAL_WORKING_DIRECTORY);
	}

	fs::path getSandboxPath() const {
		return INITIAL_WORKING_DIRECTORY / SANDBOX_NAME;
	}

	fs::path getSaveDirectoryPath() const {
		const auto sandboxPath = getSandboxPath();
		return sandboxPath / SAVE_DIRECTORY_NAME;
	}

	// -------------------------------------------------------------------------------------
	// SETUP AND TEARDOWN (FOR WORKING DIRECTORY)

	virtual void SetUp() override {
		setupSandbox();
		workInSandbox();
	}

	virtual void TearDown() override {
		returnToInitialWorkingDirectory();
		teardownSandBox();
	}

	// -------------------------------------------------------------------------------------
	// CONTROLLER UTILITY METHODS

	void makeSingleTurmiteController(const Turmite& turmite) {
		turmites_sim::TurmiteSimulator simulator;
		simulator.addTurmite(turmite);
		simulator.setGridSize(DEFAULT_GRID_SIZE);

		controller_ = TurmiteSimulatorController();
		controller_.setSimulator(simulator);
	}

	void resetController() {
		controller_ = TurmiteSimulatorController();
	}

	Turmite getFirstTurmite() {
		if (!controller_.getSimulator())
			throw std::logic_error("No simulator associated with controller.");

		const auto& turmites = controller_.getSimulator()->getTurmites();
		if (turmites.empty())
			throw std::logic_error("No turmites on grid.");

		return *turmites.begin();
	}

	TurmiteSimulatorController controller_;
};

TEST_F(TurmiteSimulatorControllerTest, saving_turmite_creates_save_dir_in_cwd_if_it_does_not_exist) {
	makeSingleTurmiteController(getLangtonsAntTransitionTable());

	ASSERT_FALSE(fs::exists(getSaveDirectoryPath()));

	controller_.saveTurmite(DEFAULT_SAVE_NAME);

	ASSERT_TRUE(fs::exists(getSaveDirectoryPath()));
}

TEST_F(TurmiteSimulatorControllerTest, loading_turmite_restores_state_transition_table) {
	
	makeSingleTurmiteController(getLangtonsAntTransitionTable());
	const auto originalTurmite = getFirstTurmite();
	controller_.saveTurmite("test_turmite_langton");

	resetController();

	controller_.loadTurmite("test_turmite_langton");
	const auto loadedTurmite = getFirstTurmite();

	const auto& originalTable = originalTurmite.getStateTransitionTable();
	const auto& loadedTable = loadedTurmite.getStateTransitionTable();
	EXPECT_EQ(originalTable, loadedTable);
}

}
