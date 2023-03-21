//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "../lib/doctests.h"
#include "engine.h"
#include "constants.h"

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
	initialize_engine();
	Position p;
	Position::set(fen, p);
	return perft_filtered<WHITE>(p, depth, options);
}

unsigned long long test_perft_generated(const std::string& fen, int depth, MoveGenerationOptions& options) {
	initialize_engine();
	Position p;
	Position::set(fen, p);
	return perft_generated<WHITE>(p, depth, options);
}

TEST_SUITE_BEGIN("MoveGenerationOptionsTesting");

TEST_CASE("StartPos Captures"){
	MoveGenerationOptions generate_captures = {
			.generate_captures = true,
	};
	int depth = 7;
	unsigned long long filtered = test_perft_filtered(INITIAL_BOARD_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(INITIAL_BOARD_FEN, depth, generate_captures);
	CHECK_EQ(filtered, generated);
	CHECK_EQ(filtered, 108329926);
}

TEST_CASE("CPW Position3 Captures"){
	MoveGenerationOptions generate_captures = {
			.generate_captures = true,
	};
	int depth = 8;
	unsigned long long filtered = test_perft_filtered("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", depth, generate_captures);
	unsigned long long generated = test_perft_generated("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", depth, generate_captures);
	CHECK_EQ(filtered, generated);
	CHECK_EQ(filtered, 267586558);
}

TEST_CASE("KiwiPete Captures"){
	MoveGenerationOptions generate_captures = {
		.generate_captures = true,
	};
	int depth = 5;
	unsigned long long filtered = test_perft_filtered(KIWIPETE_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(KIWIPETE_FEN, depth, generate_captures);
	CHECK_EQ(filtered, generated);
	CHECK_EQ(filtered, 35043416);
}

TEST_CASE("CPW Position 4 Captures") {
	MoveGenerationOptions generate_captures = {
			.generate_captures = true,
	};
	int depth = 5;
	unsigned long long filtered = test_perft_filtered(PERFT_RESULTS_POS4_FEN, depth, generate_captures);
	unsigned long long generated = test_perft_generated(PERFT_RESULTS_POS4_FEN, depth, generate_captures);
	CHECK_EQ(filtered, generated);
	CHECK_EQ(filtered, 2046173);
}

TEST_SUITE_END();
