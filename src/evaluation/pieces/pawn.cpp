//
// Created by Archishmaan Peyyety on 5/18/23.
//
//
// Created by Archishmaan Peyyety on 2/24/23.
//
#include "pawn.h"

template<Color color, DoTrace do_trace>
Score evaluate_pawn_structure(const Position& board, Trace& trace) {
	Score score = SCORE_ZERO;

	Bitboard pawns = board.occupancy<color, PAWN>();
	const Bitboard all_pawns = pawns;
	const Bitboard us_pieces = board.occupancy<color>();
	const Bitboard them_pieces = board.occupancy<~color>();
	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	// Doubled pawn eval
	score += DOUBLED_PAWN_PENALTY * pop_count(doubled_pawns<color>(board));

	// Isolate pawn eval
	score += ISOLATED_PAWN_PENALTY * pop_count(isolated_pawns<color>(board));

	// Evaluate pawn locations
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

	// Pawn Phalanx Eval
	Bitboard all_phalanx_pawns = phalanx_pawns<color>(board);
	while (all_phalanx_pawns) {
		Square pawn = pop_lsb(all_phalanx_pawns);
		const Rank relative_pawn_rank = relative_rank<color>(rank_of(pawn));
		score += PHALANX_PAWN[relative_pawn_rank];
	}

	// Passed Pawn Eval
	Bitboard passed_pawns_us = passed_pawns<color>(board);
	Bitboard candidate_passed_pawns = candidate_pawns<color>(board, passed_pawns_us);
	while (passed_pawns_us) {
		Square pawn = pop_lsb(passed_pawns_us);
		const Square relative_pawn_square = color == WHITE ? flip(pawn) : pawn;
		score += PASSED_PAWN_BONUS[relative_pawn_square];
		const bool blocked = shift_relative<color, NORTH>(square_to_bitboard(pawn)) & board.occupancy<~color>();
		score += BLOCKED_PASSED_PAWN_PENALTY[relative_pawn_square] * blocked;
	}

	// Candidate Pawn Eval
	while (candidate_passed_pawns) {
		Square pawn = pop_lsb(candidate_passed_pawns);
		Rank relative_pawn_rank = relative_rank<color>(rank_of(pawn));
		score += CANDIDATE_PASSED_PAWN[relative_pawn_rank];
	}
	return score;
}

template Score evaluate_pawn_structure<WHITE, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_pawn_structure<BLACK, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_pawn_structure<WHITE, COMPUTE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_pawn_structure<BLACK, COMPUTE_EVAL>(const Position& board, Trace& trace);
