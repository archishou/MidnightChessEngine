#pragma once
#include "move_generation/position.h"
#include "evaluation/constants/misc.h"
#include "evaluation/constants/psts.h"
#include "evaluation/pieces/pawn.h"
#include "evaluation/pieces/knight.h"
#include "evaluation/pieces/bishop.h"
#include "evaluation/pieces/rook.h"
#include "evaluation/pieces/queen.h"
#include "evaluation/pieces/king.h"
#include "types.h"

template<Color color>
constexpr int compute_game_phase(Position& board) {
	int game_phase = 0;
	for (PieceType piece_type : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		Bitboard piece_bitboard_us = board.bitboard_of(color, piece_type);
		Bitboard piece_bitboard_them = board.bitboard_of(~color, piece_type);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_us);
		game_phase += GAME_PHASE_BONUS[piece_type] * pop_count(piece_bitboard_them);
	}
	return game_phase;
}

template<Color color>
constexpr Score evaluate_single_side(Position& board) {
	return 	evaluate_pawn_structure<color>(board) +
			evaluate_knight<color>(board) +
			evaluate_bishops<color>(board) +
			evaluate_rooks<color>(board) +
			evaluate_queens<color>(board) +
			evaluate_king<color>(board);
}

template<Color color>
constexpr int evaluate(Position& board) {
	const int game_phase = compute_game_phase<color>(board);

	Score us = evaluate_single_side<color>(board);
	Score them = evaluate_single_side<~color>(board);
	Score total = us - them + TEMPO;

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = mg_value(total);
	const int eg_score = eg_value(total);
	return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}
