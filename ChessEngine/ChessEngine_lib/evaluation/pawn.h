//
// Created by Archishmaan Peyyety on 2/24/23.
//

#include "move_generation/types.h"

template<Color color>
constexpr Bitboard pawn_attack_span(const Bitboard s) {
	return shift<relative_dir<color>(NORTH)> (
			shift<EAST>(forward_files<color>(s)) |
			shift<WEST>(forward_files<color>(s))
	);
}

template<Color color>
constexpr Bitboard pawn_passed_span(const Bitboard s) {
	return shift<relative_dir<color>(NORTH)>(forward_files<color>(s) | pawn_attack_span<color>(s));
}

template<Color c>
constexpr Bitboard passed_pawns(Position& board) {
	Bitboard them_pawn_occupants = pawn_passed_span<~c>(board.bitboard_of(~c, PAWN));
	return board.bitboard_of(c, PAWN) & ~them_pawn_occupants;
}

template<Color c>
constexpr int evaluate_passed_pawns(Position& board) {
	int passed_pawn_score = 0;
	Bitboard passed_pawns_us = passed_pawns<c>(board);
	while (passed_pawns_us) {
		const Square pawn = pop_lsb(&passed_pawns_us);
		const Rank rank = relative_rank<c>(rank_of(pawn));
		passed_pawn_score += rank * PASSED_PAWN_BONUS;
	}
	return passed_pawn_score;
}

template<Color c>
constexpr int evaluate_pawn_structure(Position& board) {
	const int passed_pawn_eval = evaluate_passed_pawns<c>(board) - evaluate_passed_pawns<~c>(board);
	return passed_pawn_eval;
}