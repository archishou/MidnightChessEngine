//
// Created by Archishmaan Peyyety on 1/10/23.
//
#include "gtest/gtest.h"
#include "move_generation/position.h"

class MoveGenerationFixture : public ::testing::Test {

protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {
	}
	std::string initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};

template<Color Us>
unsigned long long perft(Position& p, unsigned int depth) {
	int nmoves;
	unsigned long long nodes = 0;

	MoveList<Us> list(p);
	if (depth == 1) return (unsigned long long) list.size();
	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft<~Us>(p, depth - 1);
		p.undo<Us>(move);
	}
	return nodes;
}

unsigned long long test_perft(const std::string& initial_fen, int depth) {
	Position p;
	Position::set(initial_fen, p);
	return perft<WHITE>(p, depth);
}

TEST_F(MoveGenerationFixture, PerftDepthOneDefaultFen){
	EXPECT_EQ(test_perft(initial_position_fen, 1), 20);
}

TEST_F(MoveGenerationFixture, PerftDepthTwoDefaultFen){
	EXPECT_EQ(test_perft(initial_position_fen, 2), 400);
}

TEST_F(MoveGenerationFixture, PerftDepthThreeDefaultFen){
	EXPECT_EQ(test_perft(initial_position_fen, 3), 8902);
}

TEST_F(MoveGenerationFixture, PerftDepthFourDefaultFen){
	EXPECT_EQ(test_perft(initial_position_fen, 4), 197281);
}

TEST_F(MoveGenerationFixture, PerftDepthFiveDefaultFen){
	EXPECT_EQ(test_perft(initial_position_fen, 5), 4865609);
}

TEST_F(MoveGenerationFixture, PerftDepthSixDefaultFen){
	EXPECT_EQ(test_perft(initial_position_fen, 6), 119060324);
}
