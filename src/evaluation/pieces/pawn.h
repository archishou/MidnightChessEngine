//
// Created by Archishmaan Peyyety on 2/24/23.
//
#pragma once
#include "move_generation/types.h"
#include "evaluation/bitboards.h"

template<Color c>
constexpr Score evaluate_passed_pawns(Position& board) {
	Score passed_pawn_score = SCORE_ZERO;
	Bitboard passed_pawns_us = passed_pawns<c>(board);
	while (passed_pawns_us) {
		Square pawn = pop_lsb(&passed_pawns_us);
		const Square relative_pawn_square = c == WHITE ? ~pawn : pawn;
		const Rank rank = relative_rank<c>(rank_of(pawn));
		passed_pawn_score += PASSED_PAWN_BONUS[relative_pawn_square];
		const bool blocked = shift<relative_dir<c>(NORTH)>(SQUARE_BB[pawn]) & board.all_pieces<~c>();
		passed_pawn_score += BLOCKED_PASSED_PAWN_PENALTY[relative_pawn_square] * blocked;
	}
	return passed_pawn_score;
}

template<Color c>
inline constexpr Score evaluate_doubled_pawns(Position& board) {
	return DOUBLED_PAWN_PENALTY * pop_count(doubled_pawns<c>(board));
}

template<Color c>
inline constexpr Score evaluate_isolated_pawns(Position& board) {
	return ISOLATED_PAWN_PENALTY * pop_count(isolated_pawns<c>(board));
}

template<Color c>
inline constexpr Score evaluate_pawn_locations(Position& board) {
	Score score = SCORE_ZERO;
	Bitboard pawns = board.bitboard_of(c, PAWN);
	const Bitboard all_pawns = pawns;
	const Bitboard us_pieces = board.all_pieces<c>();
	const Bitboard them_pieces = board.all_pieces<~c>();
	const Square them_king = bsf(board.bitboard_of(~c, KING));
	const Bitboard them_king_ring = KING_ATTACKS[them_king] & ~them_pieces;

	while (pawns) {
		Square pawn_square = pop_lsb(&pawns);
		const Rank rank = relative_rank<c>(rank_of(pawn_square));
		score += PIECE_VALUES[PAWN];
		score += read_psqt<c, PAWN>(pawn_square);

		const Bitboard supporting_pawns = all_pawns & pawn_attacks<~c>(pawn_square);
		score += PAWN_PROTECTION[PAWN] * pop_count(supporting_pawns) * rank;

		const Bitboard pseudo_legal_moves = pawn_attacks<c>(pawn_square) & ~us_pieces;
		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[PAWN] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & SQUARE_BB[them_king];
		score += CHECK_BONUS[PAWN] * king_in_check;
	}
	return score;
}

template<Color c>
constexpr Score evaluate_pawn_structure(Position& board) {
	const Score pawn_location_eval = evaluate_pawn_locations<c>(board);
	const Score passed_pawn_eval = evaluate_passed_pawns<c>(board);
	const Score isolated_pawn_eval = evaluate_isolated_pawns<c>(board);
	const Score doubled_pawn_eval = evaluate_doubled_pawns<c>(board);
	return pawn_location_eval + passed_pawn_eval + isolated_pawn_eval + doubled_pawn_eval;
}
