//
// Created by Archishmaan Peyyety on 5/17/23.
//
#include "bishop.h"

template<Color color, DoTrace do_trace>
Score evaluate_bishops(const Position &board, Trace &trace) {
	Score score = SCORE_ZERO;

	Bitboard bishops = board.occupancy<color, BISHOP>();
	Bitboard us_pieces = board.occupancy<color>();
	Bitboard them_pieces = board.occupancy<~color>();
	const Bitboard all_pawns = board.occupancy<color, PAWN>();
	const Bitboard them_pawns = board.occupancy<~color, PAWN>();
	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);
	const Bitboard xray_occupancy = us_pieces ^ bishops ^ board.occupancy<color, QUEEN>();

	const bool bishop_pair = pop_count(bishops) >> 1;
	score += BISHOP_PAIR_BONUS * bishop_pair;
	if constexpr (do_trace) trace.bishop_bonus[color] += bishop_pair;

	while (bishops) {
		Square bishop_square = pop_lsb(bishops);
		Rank rank = rank_of(bishop_square);

		score += PIECE_VALUES[BISHOP];
		if constexpr (do_trace) trace.material[BISHOP][color] += 1;

		score += read_psqt<color, BISHOP>(bishop_square);
		if constexpr (do_trace) {
			trace.bishop_pst[color == WHITE ? flip(bishop_square) : bishop_square][color] += 1;
		}

		Bitboard pseudo_legal_moves = tables::attacks<BISHOP>(bishop_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = tables::attacks<BISHOP>(bishop_square, them_pieces | xray_occupancy) & ~(xray_occupancy | them_pawn_attacks);
		Bitboard forward_mobility = mobility_squares & FORWARD_RANK_TABLE[rank][color];

		score += BISHOP_MOBILITY[pop_count(mobility_squares)];
		if constexpr (do_trace) trace.bishop_mobility[pop_count(mobility_squares)][color] += 1;

		score += BISHOP_FORWARD_MOBILITY[pop_count(forward_mobility)];
		if constexpr (do_trace) trace.bishop_forward_mobility[pop_count(forward_mobility)][color] += 1;

		const Bitboard supporting_pawns = all_pawns & tables::attacks<PAWN, ~color>(bishop_square);
		score += PAWN_PROTECTION[BISHOP] * pop_count(supporting_pawns);
		if constexpr (do_trace) trace.pawn_protection[BISHOP][color] += pop_count(supporting_pawns);

		const Bitboard attacking_pawns = them_pawns & tables::attacks<PAWN, color>(bishop_square);
		score += ATTACKED_BY_PAWN[BISHOP] * pop_count(attacking_pawns);
		if constexpr (do_trace) trace.attacked_by_pawn[BISHOP][color] += pop_count(attacking_pawns);

		const Bitboard attacked_knights = pseudo_legal_moves & board.occupancy<~color, KNIGHT>();
		score += read_threat_bonus<BISHOP, KNIGHT>() * pop_count(attacked_knights);
		if constexpr (do_trace) trace.threats[BISHOP * (NPIECE_TYPES - 1) + KNIGHT][color] += pop_count(attacked_knights);

		const Bitboard attacked_rooks = pseudo_legal_moves & board.occupancy<~color, ROOK>();
		score += read_threat_bonus<BISHOP, ROOK>() * pop_count(attacked_rooks);
		if constexpr (do_trace) trace.threats[BISHOP * (NPIECE_TYPES - 1) + ROOK][color] += pop_count(attacked_rooks);

		const Bitboard attacked_queens = pseudo_legal_moves & board.occupancy<~color, QUEEN>();
		score += read_threat_bonus<BISHOP, QUEEN>() * pop_count(attacked_queens);
		if constexpr (do_trace) trace.threats[BISHOP * (NPIECE_TYPES - 1) + QUEEN][color] += pop_count(attacked_queens);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[BISHOP] * pop_count(king_ring_attacks);
		if constexpr (do_trace) trace.king_ring_bonus[BISHOP][color] += pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
		score += CHECK_BONUS[BISHOP] * king_in_check;
		if constexpr (do_trace) trace.check_bonus[BISHOP][color] += king_in_check;

		score += CENTER_CONTROL[BISHOP] * pop_count(pseudo_legal_moves & BOARD_CENTER);
		if constexpr (do_trace) trace.center_control[BISHOP][color] += pop_count(pseudo_legal_moves & BOARD_CENTER);
	}
	return score;
}

template Score evaluate_bishops<WHITE, TRACE_EVAL>(const Position &board, Trace &trace);
template Score evaluate_bishops<BLACK, TRACE_EVAL>(const Position &board, Trace &trace);
template Score evaluate_bishops<WHITE, COMPUTE_EVAL>(const Position &board, Trace &trace);
template Score evaluate_bishops<BLACK, COMPUTE_EVAL>(const Position &board, Trace &trace);
