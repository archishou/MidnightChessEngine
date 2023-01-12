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
	const std::string initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	const std::string kiwipete_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
	const std::string talkchess_fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
};

void print_hash_history(Position& board) {
	std::cout << "HASH HISTORY START" << std::endl;
	for (uint64_t hash : board.hash_history) {
		std::cout << hash << std::endl;
	}
	std::cout << "HASH HISTORY END" << std::endl;
}

TEST_F(MoveGenerationDrawDetectionFixture, TestDrawDetection){
	Position board;
	Position::set(initial_position_fen, board);
}
