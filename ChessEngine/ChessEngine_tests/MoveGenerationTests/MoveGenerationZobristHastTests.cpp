//
// Created by Archishmaan Peyyety on 1/22/23.
//

#include "gtest/gtest.h"
#include "bitset"
#include "move_generation/position.h"

class MoveGenerationZobristHashTests : public ::testing::Test {

protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {}
	const std::string initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	const std::string kiwipete_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	const std::string talkchess_fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
};

template<Color Us>
void perft_hash(Position& p, unsigned int depth) {
	MoveList<Us> list(p);
	if (depth == 1) return;
	for (Move move : list) {
		zobrist_hash initial_hash = p.get_hash();
		p.play<Us>(move);
		perft_hash<~Us>(p, depth - 1);
		p.undo<Us>(move);
		EXPECT_EQ(initial_hash, p.get_hash());
	}
}

void test_perft_hash(const std::string& fen, int depth) {
	Position p;
	Position::set(fen, p);
	if (p.turn() == BLACK) perft_hash<BLACK>(p, depth);
	else perft_hash<WHITE>(p, depth);
}

TEST_F(MoveGenerationZobristHashTests, TestCastlingState){
	Position board;
	Position::set(initial_position_fen, board);

	for (int i = 15; i >= 0; i--) {
		std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w ";
		std::string castling_rights = "";
		std::string move_counter = " 0 1";
		if ((i >> 3) & 0b1)	castling_rights += "K";
		if ((i >> 2) & 0b1)	castling_rights += "Q";
		if ((i >> 1) & 0b1)	castling_rights += "k";
		if (i & 0b1) castling_rights += "q";
		if (i == 0) castling_rights = "-";
		std::string test_fen = fen + castling_rights + move_counter;
		Position::set(test_fen, board);
		EXPECT_EQ(board.castling_state(), i);
	}
}
TEST_F(MoveGenerationZobristHashTests, TestConsitentHashInitialFen){
	test_perft_hash(initial_position_fen, 6);
}
TEST_F(MoveGenerationZobristHashTests, TestConsitentHashKiwiPete){
	test_perft_hash(kiwipete_fen, 5);
}
TEST_F(MoveGenerationZobristHashTests, TestConsitentHashTalkchess){
	test_perft_hash(talkchess_fen, 5);
}
