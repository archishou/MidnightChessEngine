//
// Created by Archishmaan Peyyety on 7/8/23.
//

#include "evaluate.h"

template<Color color>
std::pair<Score, i32> compute_piece_count_phase(const Position& board) {
	i32 game_phase = 0;
	Score material = 0;
	for (PieceType piece_type : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		Bitboard piece_bitboard_us = board.occupancy(color, piece_type);
		Bitboard piece_bitboard_them = board.occupancy(~color, piece_type);
		i32 us = pop_count(piece_bitboard_us);
		i32 them = pop_count(piece_bitboard_them);
		game_phase += GAME_PHASE_BONUS[piece_type] * us;
		game_phase += GAME_PHASE_BONUS[piece_type] * them;
		material += PIECE_VALUES[piece_type] * (us + them);
	}
	return {material, game_phase};
}

SharedEvalFeatures generate_shared_eval(const Position& board) {
	SharedEvalFeatures eval_features{};

	const Square white_king_square = lsb(board.occupancy<WHITE, KING>());
	const Square black_king_square = lsb(board.occupancy<BLACK, KING>());
	const Bitboard pawn_occ = board.occupancy<PAWN>();

	eval_features.king_virtual_mobility[WHITE] = tables::attacks<QUEEN>(white_king_square, board.occupancy<WHITE>() | pawn_occ);
	eval_features.king_virtual_mobility[BLACK] = tables::attacks<QUEEN>(black_king_square, board.occupancy<BLACK>() | pawn_occ);
	return eval_features;
}

i32 scale_eval(i32 eval, Score material) {
	return (eval * (700 + material / 32)) / 1024;
}

template<Color color, DoTrace do_trace>
Score evaluate_single_side(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace) {
	return evaluate_un_cachable_pawn_struct<color, do_trace>(board, eval_features, trace) +
		   evaluate_knight<color, do_trace>(board, eval_features, trace) +
		   evaluate_bishops<color, do_trace>(board, eval_features, trace) +
		   evaluate_rooks<color, do_trace>(board, eval_features, trace) +
		   evaluate_queens<color, do_trace>(board, eval_features, trace) +
		   evaluate_king<color, do_trace>(board, eval_features, trace);
}

template std::pair<Score, i32> compute_piece_count_phase<WHITE>(const Position &board);
template std::pair<Score, i32> compute_piece_count_phase<BLACK>(const Position &board);

template Score
evaluate_single_side<WHITE, TRACE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
template Score
evaluate_single_side<BLACK, TRACE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
template Score
evaluate_single_side<WHITE, COMPUTE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
template Score
evaluate_single_side<BLACK, COMPUTE_EVAL>(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
