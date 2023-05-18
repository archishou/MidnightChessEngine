//
// Created by Archishmaan Peyyety on 5/18/23.
//
//
// Created by Archishmaan Peyyety on 2/24/23.
//
#include "evaluation/bitboards.h"
#include "pawn.h"
#include "evaluation/types.h"
#include "evaluation/constants/psts.h"
#include "evaluation/constants/misc.h"

template<Color color>
Score evaluate_passed_pawns(const Position& board) {
	Score passed_pawn_score = SCORE_ZERO;
	Bitboard passed_pawns_us = passed_pawns<color>(board);
	Bitboard candidate_passed_pawns = candidate_pawns<color>(board, passed_pawns_us);
	while (passed_pawns_us) {
		Square pawn = pop_lsb(passed_pawns_us);
		const Square relative_pawn_square = color == WHITE ? flip(pawn) : pawn;
		passed_pawn_score += PASSED_PAWN_BONUS[relative_pawn_square];
		const bool blocked = shift_relative<color, NORTH>(square_to_bitboard(pawn)) & board.occupancy<~color>();
		passed_pawn_score += BLOCKED_PASSED_PAWN_PENALTY[relative_pawn_square] * blocked;
	}
	while (candidate_passed_pawns) {
		Square pawn = pop_lsb(candidate_passed_pawns);
		Rank relative_pawn_rank = relative_rank<color>(rank_of(pawn));
		passed_pawn_score += CANDIDATE_PASSED_PAWN[relative_pawn_rank];
	}
	return passed_pawn_score;
}

template<Color color>
Score evaluate_pawn_phalanx(const Position& board) {
	Score phalanx_pawn_score = SCORE_ZERO;
	Bitboard all_phalanx_pawns = phalanx_pawns<color>(board);
	while (all_phalanx_pawns) {
		Square pawn = pop_lsb(all_phalanx_pawns);
		const Rank relative_pawn_rank = relative_rank<color>(rank_of(pawn));
		phalanx_pawn_score += PHALANX_PAWN[relative_pawn_rank];
	}
	return phalanx_pawn_score;
}

template<Color color>
Score evaluate_pawn_locations(const Position& board) {
	Score score = SCORE_ZERO;
	Bitboard pawns = board.occupancy<color, PAWN>();
	const Bitboard all_pawns = pawns;
	const Bitboard us_pieces = board.occupancy<color>();
	const Bitboard them_pieces = board.occupancy<~color>();
	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	while (pawns) {
		Square pawn_square = pop_lsb(pawns);
		const Rank rank = relative_rank<color>(rank_of(pawn_square));
		score += PIECE_VALUES[PAWN];
		score += read_psqt<color, PAWN>(pawn_square);

		const Bitboard supporting_pawns = all_pawns & tables::attacks<PAWN, ~color>(pawn_square);
		score += PAWN_PROTECTION[PAWN] * pop_count(supporting_pawns) * rank;

		const Bitboard pseudo_legal_moves = tables::attacks<PAWN, color>(pawn_square) & ~us_pieces;
		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[PAWN] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
		score += CHECK_BONUS[PAWN] * king_in_check;
	}
	return score;
}

template<Color color>
Score evaluate_pawn_structure(const Position& board) {
	const Score pawn_location_eval = evaluate_pawn_locations<color>(board);
	const Score passed_pawn_eval = evaluate_passed_pawns<color>(board);
	const Score isolated_pawn_eval = evaluate_isolated_pawns<color>(board);
	const Score doubled_pawn_eval = evaluate_doubled_pawns<color>(board);
	const Score phalanx_pawn_eval = evaluate_pawn_phalanx<color>(board);
	return pawn_location_eval + passed_pawn_eval + isolated_pawn_eval + doubled_pawn_eval + phalanx_pawn_eval;
}

template Score evaluate_passed_pawns<WHITE>(const Position& board);
template Score evaluate_passed_pawns<BLACK>(const Position& board);

template Score evaluate_pawn_phalanx<WHITE>(const Position& board);
template Score evaluate_pawn_phalanx<BLACK>(const Position& board);

template Score evaluate_pawn_locations<WHITE>(const Position& board);
template Score evaluate_pawn_locations<BLACK>(const Position& board);

template Score evaluate_pawn_structure<WHITE>(const Position& board);
template Score evaluate_pawn_structure<BLACK>(const Position& board);
