#pragma once
#include "types.h"
#include "../board/position.h"
#include "../board/types/bitboard.h"
#include "constants/constants.h"
#include "pieces/pawn.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "iostream"

template<Color color>
constexpr int compute_game_phase(const Position& board) {
	int game_phase = 0;
	for (PieceType piece_type : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		Bitboard piece_bitboard_us = board.occupancy(color, piece_type);
		Bitboard piece_bitboard_them = board.occupancy(~color, piece_type);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_us);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_them);
	}
	return game_phase;
}

inline SharedEvalFeatures generate_shared_eval(const Position& board) {
	SharedEvalFeatures eval_features{};

	const Square white_king_square = lsb(board.occupancy<WHITE, KING>());
	const Square black_king_square = lsb(board.occupancy<BLACK, KING>());
	const Bitboard pawn_occ = board.occupancy<PAWN>();

	eval_features.king_virtual_mobility[WHITE] = tables::attacks<QUEEN>(white_king_square, board.occupancy<WHITE>() | pawn_occ);
	eval_features.king_virtual_mobility[BLACK] = tables::attacks<QUEEN>(black_king_square, board.occupancy<BLACK>() | pawn_occ);
	return eval_features;
}

template<Color color, DoTrace do_trace>
constexpr Score evaluate_single_side(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace) {
	return evaluate_pawn_structure<color, do_trace>(board, eval_features, trace) +
		   evaluate_knight<color, do_trace>(board, eval_features, trace) +
		   evaluate_bishops<color, do_trace>(board, eval_features, trace) +
		   evaluate_rooks<color, do_trace>(board, eval_features, trace) +
			evaluate_queens<color, do_trace>(board, eval_features, trace) +
			evaluate_king<color, do_trace>(board, eval_features, trace);
}

template<Color color, typename do_trace = DisableTrace>
constexpr do_trace evaluate(const Position& board) {
	Trace trace{};

	constexpr bool trace_enabled = std::is_same_v<do_trace, EnableTrace>;

	const int game_phase = compute_game_phase<color>(board);
	const SharedEvalFeatures eval_features = generate_shared_eval(board);

	Score us = evaluate_single_side<color, trace_enabled>(board, eval_features, trace);
	Score them = evaluate_single_side<~color, trace_enabled>(board, eval_features, trace);
	Score total = us - them + TEMPO;
	if constexpr (trace_enabled) trace.tempo[color] += 1;

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = mg_value(total);
	const int eg_score = eg_value(total);
	int eval = (mg_score * mg_phase + eg_score * eg_phase) / 24;

	if constexpr (trace_enabled) {
		if constexpr (color == BLACK) eval = -eval;
		trace.score = eval;
		return trace;
	} else {
		return eval;
	}
}
