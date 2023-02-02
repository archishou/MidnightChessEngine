//
// Created by Archishmaan Peyyety on 1/11/23.
//

#include "gtest/gtest.h"
#include "move_generation/position.h"

class MoveGenerationDrawDetectionFixture : public ::testing::Test {

protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {
	}
};

TEST_F(MoveGenerationDrawDetectionFixture, TestDrawDetection){}
