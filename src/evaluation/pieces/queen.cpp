//
// Created by Archishmaan Peyyety on 5/17/23.
//
#include "queen.h"

template<Color color, DoTrace do_trace>
Score evaluate_queens(const Position& board, Trace& trace) {
	Bitboard queens = board.occupancy<color, QUEEN>();
	const Bitboard us_pieces = board.occupancy<color>();
	const Bitboard them_pieces = board.occupancy<~color>();

	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);

	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	const Bitboard them_pawns = board.occupancy<~color, PAWN>();

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);
	const Bitboard xray_occupancy = us_pieces ^ queens ^ board.occupancy<color, ROOK>() ^ board.occupancy<color, BISHOP>();
	Score score = SCORE_ZERO;

	while (queens) {
		Square queen_square = pop_lsb(queens);
		const Bitboard queen_square_bb = square_to_bitboard(queen_square);

		score += PIECE_VALUES[QUEEN];
		if constexpr (do_trace) trace.material[QUEEN][color] += 1;

		score += read_psqt<color, QUEEN>(queen_square);
		if constexpr (do_trace) trace.queen_pst[color == WHITE ? flip(queen_square) : queen_square][color] += 1;

		Bitboard pseudo_legal_moves = tables::attacks<QUEEN>(queen_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = tables::attacks<QUEEN>(queen_square, them_pieces | xray_occupancy) & ~(xray_occupancy | them_pawn_attacks);

		score += QUEEN_MOBILITY[pop_count(mobility_squares)];
		if constexpr (do_trace) trace.queen_mobility[pop_count(mobility_squares)][color] += 1;

		const bool on_open_file = queen_square_bb & board_open_files;
		const bool on_semi_open_file = queen_square_bb & board_semi_open_files;
		score += (OPEN_FILE_BONUS[QUEEN] * on_open_file) + (SEMI_OPEN_FILE_BONUS[QUEEN] * on_semi_open_file);
		if constexpr (do_trace) {
			trace.open_files[QUEEN][color] += on_open_file;
			trace.semi_open_files[QUEEN][color] += on_semi_open_file;
		}

		const Bitboard attacking_pawns = them_pawns & tables::attacks<PAWN, color>(queen_square);
		score += ATTACKED_BY_PAWN[QUEEN] * pop_count(attacking_pawns);
		if constexpr (do_trace) trace.attacked_by_pawn[QUEEN][color] += pop_count(attacking_pawns);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[QUEEN] * pop_count(king_ring_attacks);
		if constexpr (do_trace) trace.king_ring_bonus[QUEEN][color] += pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
		score += CHECK_BONUS[QUEEN] * king_in_check;
		if constexpr (do_trace) trace.check_bonus[QUEEN][color] += king_in_check;

		score += CENTER_CONTROL[QUEEN] * pop_count(pseudo_legal_moves & BOARD_CENTER);
		if constexpr (do_trace) trace.center_control[QUEEN][color] += pop_count(pseudo_legal_moves & BOARD_CENTER);
	}
	return score;
}

template Score evaluate_queens<WHITE, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_queens<BLACK, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_queens<WHITE, COMPUTE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_queens<BLACK, COMPUTE_EVAL>(const Position& board, Trace& trace);