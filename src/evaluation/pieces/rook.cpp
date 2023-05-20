//
// Created by Archishmaan Peyyety on 5/17/23.
//
#include "rook.h"

template<Color color, DoTrace do_trace>
Score evaluate_rooks(const Position& board, Trace& trace) {
	Score score = SCORE_ZERO;
	Bitboard rooks = board.occupancy<color, ROOK>();

	Bitboard us_pieces = board.occupancy<color>();
	Bitboard them_pieces = board.occupancy<~color>();
	const Bitboard them_pawns = board.occupancy<~color, PAWN>();
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);
	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);
	const Bitboard xray_occupancy = us_pieces ^ rooks ^ board.occupancy<color, QUEEN>();
	while (rooks) {
		Square rook_square = pop_lsb(rooks);
		const Bitboard rook_square_bb = square_to_bitboard(rook_square);

		score += PIECE_VALUES[ROOK];
		if constexpr (do_trace) trace.material[ROOK][color] += 1;

		score += read_psqt<color, ROOK>(rook_square);
		if constexpr (do_trace) trace.rook_pst[color == WHITE ? flip(rook_square) : rook_square][color] += 1;

		Bitboard pseudo_legal_moves = tables::attacks<ROOK>(rook_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = tables::attacks<ROOK>(rook_square, them_pieces | xray_occupancy) & ~(xray_occupancy | them_pawn_attacks);

		score += ROOK_MOBILITY[pop_count(mobility_squares)];
		if constexpr (do_trace) trace.rook_mobility[pop_count(mobility_squares)][color] += 1;

		const bool on_open_file = rook_square_bb & board_open_files;
		const bool on_semi_open_file = rook_square_bb & board_semi_open_files;
		score += (OPEN_FILE_BONUS[ROOK] * on_open_file) + (SEMI_OPEN_FILE_BONUS[ROOK] * on_semi_open_file);
		if constexpr (do_trace) {
			trace.open_files[ROOK][color] += on_open_file;
			trace.semi_open_files[ROOK][color] += on_semi_open_file;
		}

		const Bitboard attacking_pawns = them_pawns & pawn_attacks<color>(rook_square);
		score += ATTACKED_BY_PAWN[ROOK] * pop_count(attacking_pawns);
		if constexpr (do_trace) trace.attacked_by_pawn[ROOK][color] += pop_count(attacking_pawns);

		const Bitboard attacked_queens = pseudo_legal_moves & board.occupancy<~color, QUEEN>();
		score += read_threat_bonus<ROOK, QUEEN>() * pop_count(attacked_queens);
		if constexpr (do_trace) trace.threats[ROOK * (NPIECE_TYPES - 1) + QUEEN][color] += pop_count(attacked_queens);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[ROOK] * pop_count(king_ring_attacks);
		if constexpr (do_trace) trace.king_ring_bonus[ROOK][color] += pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
		score += CHECK_BONUS[ROOK] * king_in_check;
		if constexpr (do_trace) trace.check_bonus[ROOK][color] += king_in_check;

		score += CENTER_CONTROL[ROOK] * pop_count(pseudo_legal_moves & BOARD_CENTER);
		if constexpr (do_trace) trace.center_control[ROOK][color] += pop_count(pseudo_legal_moves & BOARD_CENTER);
	}
	return score;
}

template Score evaluate_rooks<WHITE, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_rooks<BLACK, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_rooks<WHITE, COMPUTE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_rooks<BLACK, COMPUTE_EVAL>(const Position& board, Trace& trace);