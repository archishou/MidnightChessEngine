//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "../lib/doctests.h"
#include "engine.h"
#include "utils/fen_constants.h"

bool is_move_flag_promotion_cap(MoveFlag flag) {
	return flag == PC_BISHOP || flag == PC_KNIGHT || flag == PC_ROOK || flag == PC_QUEEN;
}

bool is_move_flag_promotion(MoveFlag flag) {
	return flag == PR_KNIGHT || flag == PR_BISHOP || flag == PR_ROOK || flag == PR_QUEEN;
}

template<Color color>
std::vector<Move> filter_moves(MoveList<color, ALL> &move_list) {
	std::vector<Move> filtered_moves;
	for (Move move: move_list) {
		if (move.flag() == CAPTURE ||
			is_move_flag_promotion_cap(move.flag()) ||
			is_move_flag_promotion(move.flag()) ||
			move.flag() == EN_PASSANT) {
			filtered_moves.push_back(move);
		}
	}
	return filtered_moves;
}

template<Color Us>
unsigned long long perft_filtered(Position& p, unsigned int depth) {
	int nmoves;
	unsigned long long nodes = 0;
	MoveList<Us, ALL> list(p);
	if (depth == 1) {
		std::vector<Move> captures = filter_moves<Us>(list);
		return (unsigned long long) captures.size();
	}
	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft_filtered<~Us>(p, depth - 1);
		p.undo<Us>(move);
	}
	return nodes;
}

template<Color Us>
unsigned long long perft_generated(Position& p, unsigned int depth) {
	int nmoves;
	unsigned long long nodes = 0;
	MoveList<Us, ALL> list(p);
	if (depth == 1) {
		MoveList<Us, QSEARCH> captures(p);
		return (unsigned long long) captures.size();
	}
	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft_generated<~Us>(p, depth - 1);
		p.undo<Us>(move);
	}
	return nodes;
}

unsigned long long test_perft_filtered(const std::string& fen, int depth) {
	initialize_engine();
	Position p;
	Position::set(fen, p);
	return perft_filtered<WHITE>(p, depth);
}

unsigned long long test_perft_generated(const std::string& fen, int depth) {
	initialize_engine();
	Position p;
	Position::set(fen, p);
	return perft_generated<WHITE>(p, depth);
}

TEST_SUITE_BEGIN("move-gen-options");

TEST_CASE("StartPos Captures"){
	int depth = 6;
	unsigned long long filtered = test_perft_filtered(INITIAL_BOARD_FEN, depth);
	unsigned long long generated = test_perft_generated(INITIAL_BOARD_FEN, depth);
	CHECK_EQ(filtered, generated);
}

TEST_CASE("CPW Position3 Captures"){
	int depth = 7;
	unsigned long long filtered = test_perft_filtered("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", depth);
	unsigned long long generated = test_perft_generated("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", depth);
	CHECK_EQ(filtered, generated);
}

TEST_CASE("KiwiPete Captures"){
	int depth = 5;
	unsigned long long filtered = test_perft_filtered(KIWIPETE_FEN, depth);
	unsigned long long generated = test_perft_generated(KIWIPETE_FEN, depth);
	CHECK_EQ(filtered, generated);
}

TEST_CASE("CPW Position 4 Captures") {
	int depth = 5;
	unsigned long long filtered = test_perft_filtered(PERFT_RESULTS_POS4_FEN, depth);
	unsigned long long generated = test_perft_generated(PERFT_RESULTS_POS4_FEN, depth);
	CHECK_EQ(filtered, generated);
}

TEST_SUITE_END();
