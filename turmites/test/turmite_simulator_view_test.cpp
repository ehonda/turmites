#include <filesystem>
#include <fstream>

#include "gtest/gtest.h"

#include "turmite_simulator_view.h"

using namespace turmites_sim::mvc;
namespace fs = std::filesystem;

namespace turmites_sim_test {

class TurmiteSimulatorViewTest : public testing::Test {
protected:
	const std::string TEST_COLOR_MAP_FILE = "test_colormap.txt";
	const std::string TEST_COLOR_MAP_CONTENT_STRING
		=	"FF0000""\n"
			"00FF00""\n"
			"0000FF""\n";
	const std::vector<SDL_Color> TEST_COLOR_MAP
		= {	{255, 0, 0, SDL_ALPHA_OPAQUE},
			{0, 255, 0, SDL_ALPHA_OPAQUE},
			{0, 0, 255, SDL_ALPHA_OPAQUE}
		};
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

	fs::path getColorMapDirectoryPath() const {
		const auto sandboxPath = getSandboxPath();
		return sandboxPath / COLOR_MAP_DIRECTORY_NAME;
	}

	void makeTestColorMapFile() const {
		// Create colormap directory since it doesnt exist at this point
		fs::create_directory(getColorMapDirectoryPath());

		std::ofstream testFile(getColorMapDirectoryPath() / TEST_COLOR_MAP_FILE);
		testFile << TEST_COLOR_MAP_CONTENT_STRING;
	}

	// -------------------------------------------------------------------------------------
	// SETUP AND TEARDOWN (FOR WORKING DIRECTORY)

	virtual void SetUp() override {
		setupSandbox();
		workInSandbox();
		//makeTestColorMapFile();
	}

	virtual void TearDown() override {
		returnToInitialWorkingDirectory();
		teardownSandBox();
	}

	// -------------------------------------------------------------------------------------
	// SETUP AND TEARDOWN (FOR WORKING DIRECTORY)

	void EXPECT_COLOR_MAP_EQ(const CellStateToColorMap& actual, 
		const CellStateToColorMap& expected) const 
	{
		const auto size = actual.size();
		ASSERT_EQ(size, expected.size());
		for (std::size_t i = 0; i < size; ++i) {
			ASSERT_EQ(actual[i].r, expected[i].r);
			ASSERT_EQ(actual[i].g, expected[i].g);
			ASSERT_EQ(actual[i].b, expected[i].b);
			ASSERT_EQ(actual[i].a, expected[i].a);
		}
	}
};

// MOVE LOGIC TO MAKE DIRECTORY TO APT PLACE
TEST_F(TurmiteSimulatorViewTest, loading_cmap_creates_directory_if_it_does_not_exist) {
	ASSERT_FALSE(fs::exists(getColorMapDirectoryPath()));
	readColorMapFile(TEST_COLOR_MAP_FILE);
	ASSERT_TRUE(fs::exists(getColorMapDirectoryPath()));
}

TEST_F(TurmiteSimulatorViewTest, load_test_color_map) {
	makeTestColorMapFile();
	const auto readColorMap = readColorMapFile(TEST_COLOR_MAP_FILE);
	EXPECT_COLOR_MAP_EQ(readColorMap, TEST_COLOR_MAP);
}

}
