//
// Created by Archishmaan Peyyety on 5/17/23.
//

#include "knight.h"

template<Color color, DoTrace do_trace>
Score evaluate_knight(const Position& board, Trace& trace) {
	Bitboard knights = board.occupancy<color, KNIGHT>();
	Bitboard us_pieces = board.occupancy<color>();
	Bitboard them_pieces = board.occupancy<~color>();

	const Bitboard all_pawns = board.occupancy<color, PAWN>();
	const Bitboard them_pawns = board.occupancy<~color, PAWN>();

	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);

	Score score = SCORE_ZERO;

	while (knights) {
		Square knight_square = pop_lsb(knights);

		score += PIECE_VALUES[KNIGHT];
		if constexpr (do_trace) trace.material[KNIGHT][color] += 1;

		score += read_psqt<color, KNIGHT>(knight_square);
		if constexpr (do_trace) {
			trace.knight_pst[color == WHITE ? flip(knight_square) : knight_square][color] += 1;
		}

		Bitboard pseudo_legal_moves = tables::attacks<KNIGHT>(knight_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = pseudo_legal_moves & ~them_pawn_attacks;

		score += KNIGHT_MOBILITY[pop_count(mobility_squares)];
		if constexpr (do_trace) trace.knight_mobility[pop_count(mobility_squares)][color] += 1;

		const Bitboard supporting_pawns = all_pawns & tables::attacks<PAWN, ~color>(knight_square);
		score += PAWN_PROTECTION[KNIGHT] * pop_count(supporting_pawns);
		if constexpr (do_trace) trace.pawn_protection[KNIGHT][color] += pop_count(supporting_pawns);

		const Bitboard attacking_pawns = them_pawns & tables::attacks<PAWN, color>(knight_square);
		score += ATTACKED_BY_PAWN[KNIGHT] * pop_count(attacking_pawns);
		if constexpr (do_trace) trace.attacked_by_pawn[KNIGHT][color] += pop_count(attacking_pawns);

		const Bitboard attacked_bishops = pseudo_legal_moves & board.occupancy<~color, BISHOP>();
		score += read_threat_bonus<KNIGHT, BISHOP>() * pop_count(attacked_bishops);
		if constexpr (do_trace) trace.threats[KNIGHT * NPIECE_TYPES + BISHOP][color] += pop_count(attacked_bishops);

		const Bitboard attacked_rooks = pseudo_legal_moves & board.occupancy<~color, ROOK>();
		score += read_threat_bonus<KNIGHT, ROOK>() * pop_count(attacked_rooks);
		if constexpr (do_trace) trace.threats[KNIGHT * NPIECE_TYPES + ROOK][color] += pop_count(attacked_rooks);

		const Bitboard attacked_queens = pseudo_legal_moves & board.occupancy<~color, QUEEN>();
		score += read_threat_bonus<KNIGHT, QUEEN>() * pop_count(attacked_queens);
		if constexpr (do_trace) trace.threats[KNIGHT * NPIECE_TYPES + QUEEN][color] += pop_count(attacked_queens);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[KNIGHT] * pop_count(king_ring_attacks);
		if constexpr (do_trace) trace.king_ring_bonus[KNIGHT][color] += pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
		score += CHECK_BONUS[KNIGHT] * king_in_check;
		if constexpr (do_trace) trace.check_bonus[KNIGHT][color] += king_in_check;

		score += CENTER_CONTROL[KNIGHT] * pop_count(pseudo_legal_moves & BOARD_CENTER);
		if constexpr (do_trace) trace.center_control[KNIGHT][color] += pop_count(pseudo_legal_moves & BOARD_CENTER);
	}
	return score;
}

template Score evaluate_knight<WHITE, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_knight<BLACK, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_knight<WHITE, COMPUTE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_knight<BLACK, COMPUTE_EVAL>(const Position& board, Trace& trace);