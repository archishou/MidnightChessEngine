//
// Created by Archishmaan Peyyety on 2/24/23.
//
#pragma once
#include "evaluation/bitboards.h"

template<Color c>
constexpr Score evaluate_passed_pawns(Position& board) {
	Score passed_pawn_score = SCORE_ZERO;
	Bitboard passed_pawns_us = passed_pawns<c>(board);
	Bitboard candidate_passed_pawns = candidate_pawns<c>(board, passed_pawns_us);
	while (passed_pawns_us) {
		Square pawn = pop_lsb(passed_pawns_us);
		const Square relative_pawn_square = c == WHITE ? flip(pawn) : pawn;
		passed_pawn_score += PASSED_PAWN_BONUS[relative_pawn_square];
		const bool blocked = shift_relative<c, NORTH>(square_to_bitboard(pawn)) & board.occupancy<~c>();
		passed_pawn_score += BLOCKED_PASSED_PAWN_PENALTY[relative_pawn_square] * blocked;
	}
	while (candidate_passed_pawns) {
		Square pawn = pop_lsb(candidate_passed_pawns);
		Rank relative_pawn_rank = relative_rank<c>(rank_of(pawn));
		passed_pawn_score += CANDIDATE_PASSED_PAWN[relative_pawn_rank];
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
inline constexpr Score evaluate_pawn_phalanx(Position& board) {
	Score phalanx_pawn_score = SCORE_ZERO;
	Bitboard all_phalanx_pawns = phalanx_pawns<c>(board);
	while (all_phalanx_pawns) {
		Square pawn = pop_lsb(all_phalanx_pawns);
		const Rank relative_pawn_rank = relative_rank<c>(rank_of(pawn));
		phalanx_pawn_score += PHALANX_PAWN[relative_pawn_rank];
	}
	return phalanx_pawn_score;
}

template<Color c>
inline constexpr Score evaluate_pawn_locations(Position& board) {
	Score score = SCORE_ZERO;
	Bitboard pawns = board.occupancy<c, PAWN>();
	const Bitboard all_pawns = pawns;
	const Bitboard us_pieces = board.occupancy<c>();
	const Bitboard them_pieces = board.occupancy<~c>();
	const Square them_king = lsb(board.occupancy<~c, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	while (pawns) {
		Square pawn_square = pop_lsb(pawns);
		const Rank rank = relative_rank<c>(rank_of(pawn_square));
		score += PIECE_VALUES[PAWN];
		score += read_psqt<c, PAWN>(pawn_square);

		const Bitboard supporting_pawns = all_pawns & tables::attacks<PAWN, ~c>(pawn_square);
		score += PAWN_PROTECTION[PAWN] * pop_count(supporting_pawns) * rank;

		const Bitboard pseudo_legal_moves = tables::attacks<PAWN, c>(pawn_square) & ~us_pieces;
		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[PAWN] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
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
	const Score phalanx_pawn_eval = evaluate_pawn_phalanx<c>(board);
	return pawn_location_eval + passed_pawn_eval + isolated_pawn_eval + doubled_pawn_eval + phalanx_pawn_eval;
}
