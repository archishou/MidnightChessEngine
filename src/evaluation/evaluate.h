#pragma once
#include "types.h"
#include "../board/position.h"
#include "../board/types/bitboard.h"
#include "constants/misc.h"
#include "pieces/pawn.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/king.h"

template<Color color>
constexpr int compute_game_phase(Position& board) {
	int game_phase = 0;
	for (PieceType piece_type : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		Bitboard piece_bitboard_us = board.occupancy(color, piece_type);
		Bitboard piece_bitboard_them = board.occupancy(~color, piece_type);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_us);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_them);
	}
	return game_phase;
}

template<Color color, DoTrace do_trace>
constexpr Score evaluate_single_side(Position& board, Trace& trace) {
	return evaluate_pawn_structure<color, do_trace>(board, trace) +
		   evaluate_knight<color, do_trace>(board, trace) +
		   evaluate_bishops<color, do_trace>(board, trace) +
		   evaluate_rooks<color, do_trace>(board, trace) +
		   evaluate_queens<color, do_trace>(board, trace) +
		   evaluate_king<color, do_trace>(board, trace);
}

template<Color color, typename do_trace = DisableTrace>
constexpr do_trace evaluate(Position& board) {
	Trace trace{};

	constexpr bool trace_enabled = std::is_same_v<do_trace, EnableTrace>;

	const int game_phase = compute_game_phase<color>(board);

	Score us = evaluate_single_side<color, trace_enabled>(board, trace);
	Score them = evaluate_single_side<~color, trace_enabled>(board, trace);
	Score total = us - them + TEMPO;

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = mg_value(total);
	const int eg_score = eg_value(total);
	const int eval = (mg_score * mg_phase + eg_score * eg_phase) / 24;

	if constexpr (trace_enabled) {
		return trace;
	} else {
		return eval;
	}
}
