//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "gtest/gtest.h"
#include "move_generation/position.h"
#include "constants.h"

class MoveGenerationOptionTests : public ::testing::Test {

protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {}
};

bool is_move_flag_promotion_cap(MoveFlag flag) {
	return flag == PC_BISHOP || flag == PC_KNIGHT || flag == PC_ROOK || flag == PC_QUEEN;
}

template<Color color>
std::vector<Move> filter_moves(MoveList<color> &move_list, MoveGenerationOptions& options) {
	std::vector<Move> filtered_moves;
	for (Move move: move_list) {
		if (options.generate_captures) {
			if (move.flag() == CAPTURE ||
				is_move_flag_promotion_cap(move.flag()) ||
				move.flag() == EN_PASSANT) {
				filtered_moves.push_back(move);
			}
		}
		if (options.generate_quiet) {
			if (move.flag() == QUIET ||
				move.flag() == OO ||
				move.flag() == OOO) {
				filtered_moves.push_back(move);
			}
		}
	}
	return filtered_moves;
}

template<Color Us>
unsigned long long perft_filtered(Position& p, unsigned int depth, MoveGenerationOptions& options) {
	int nmoves;
	unsigned long long nodes = 0;
	MoveList<Us> list(p);
	if (depth == 1) {
		std::vector<Move> captures = filter_moves<Us>(list, options);
		return (unsigned long long) captures.size();
	}
	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft_filtered<~Us>(p, depth - 1, options);
		p.undo<Us>(move);
	}
	return nodes;
}

template<Color Us>
unsigned long long perft_generated(Position& p, unsigned int depth, MoveGenerationOptions& options) {
	int nmoves;
	unsigned long long nodes = 0;
	MoveList<Us> list(p);
	if (depth == 1) {
		MoveList<Us> captures(p, options);
		return (unsigned long long) captures.size();
	}
	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft_generated<~Us>(p, depth - 1, options);
		p.undo<Us>(move);
	}
	return nodes;
}

unsigned long long test_perft_filtered(const std::string& fen, int depth, MoveGenerationOptions& options) {
	Position p;
	Position::set(fen, p);
	return perft_filtered<WHITE>(p, depth, options);
}

unsigned long long test_perft_generated(const std::string& fen, int depth, MoveGenerationOptions& options) {
	Position p;
	Position::set(fen, p);
	return perft_generated<WHITE>(p, depth, options);
}

TEST_F(MoveGenerationOptionTests, TestKiwiCapturesDepth4){
	MoveGenerationOptions generate_captures = {
			.generate_captures = true,
	};
	int depth = 4;
	unsigned long long filtered = test_perft_filtered(KIWIPETE_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(KIWIPETE_FEN, depth, generate_captures);
	EXPECT_EQ(filtered, generated);
	EXPECT_EQ(filtered, 757163);
}

TEST_F(MoveGenerationOptionTests, TestKiwiCapturesDepth5){
	MoveGenerationOptions generate_captures = {
		.generate_captures = true,
	};
	int depth = 5;
	unsigned long long filtered = test_perft_filtered(KIWIPETE_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(KIWIPETE_FEN, depth, generate_captures);
	EXPECT_EQ(filtered, generated);
	EXPECT_EQ(filtered, 35043416);
}

TEST_F(MoveGenerationOptionTests, TestP4CapturesDepth4){
	MoveGenerationOptions generate_captures = {
			.generate_captures = true,
	};
	int depth = 4;
	unsigned long long filtered = test_perft_filtered(PERFT_RESULTS_POS4_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(PERFT_RESULTS_POS4_FEN, depth, generate_captures);
	EXPECT_EQ(filtered, generated);
	EXPECT_EQ(filtered, 131393);
}

TEST_F(MoveGenerationOptionTests, TestP4CapturesDepth5){
	MoveGenerationOptions generate_captures = {
			.generate_captures = true,
	};
	int depth = 5;
	unsigned long long filtered = test_perft_filtered(PERFT_RESULTS_POS4_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(PERFT_RESULTS_POS4_FEN, depth, generate_captures);
	EXPECT_EQ(filtered, generated);
	EXPECT_EQ(filtered, 2046173);
}

