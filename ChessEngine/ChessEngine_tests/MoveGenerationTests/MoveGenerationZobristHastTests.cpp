//
// Created by Archishmaan Peyyety on 1/22/23.
//

#include "gtest/gtest.h"
#include "move_generation/position.h"

class MoveGenerationZobristHashTests : public ::testing::Test {

protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {
	}
	const std::string initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//const std::string kiwipete_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
	//const std::string talkchess_fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
};

void quick_play(Position& board, Move move) {
	if (board.turn() == BLACK) {
		board.play<BLACK>(move);
	} else {
		board.play<WHITE>(move);
	}
}

TEST_F(MoveGenerationZobristHashTests, TestDrawDetection){
	Position board;
	Position::set(initial_position_fen, board);
	zobrist_hash hash = board.get_hash();
	quick_play(board, Move(e2, e4, QUIET));

	zobrist_hash  hash_1 = board.get_hash();
	quick_play(board, Move(c7, c6, QUIET));
	zobrist_hash  hash_2 = board.get_hash();
	board.undo<BLACK>(Move(c7, c6, QUIET));
	zobrist_hash hash_11 = board.get_hash();
	EXPECT_EQ(hash_1, hash_11);
}
