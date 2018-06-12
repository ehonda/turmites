#include <sstream>

#include "gtest/gtest.h"

#include "state_transition_table.h"

using namespace turmites_sim;
using namespace turmite;

namespace turmites_sim_test {

class StateTransitionTableTest : public testing::Test {

};

TEST_F(StateTransitionTableTest, save_and_load_a_stt) {
	const auto langtonTable = getLangtonsAntTransitionTable();

	std::stringstream saveStream;
	saveTransitionTable(langtonTable, saveStream);

	//std::cout << saveStream.str();

	const auto loadedTable = loadTransitionTable(saveStream);
	EXPECT_EQ(langtonTable, loadedTable);
}

}