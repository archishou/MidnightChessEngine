#include "move_generation/position.h"
#include "eval_constants.h"
#include "pawn.h"

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

// Evaluates piece position and material count simultaneously
template<Color color>
constexpr Score evaluate_piece_position(Position& board, PieceType piece_type) {
	Score score;
	Bitboard piece_bitboard_us = board.bitboard_of(color, piece_type);
	while (piece_bitboard_us) {
		Square square = pop_lsb(&piece_bitboard_us);
		score += read_pstq<color>(piece_type, square) + PIECE_VALUES[piece_type];
	}
	return score;
}

template<Color color>
constexpr Score evaluate_all_piece_positions(Position& board) {
	Score score;
	for (PieceType piece_type : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
		score += evaluate_piece_position<color>(board, piece_type);
	}
	return score;
}

template<Color color>
constexpr Score evaluate_single_side(Position& board) {
	return evaluate_all_piece_positions<color>(board) + evaluate_pawn_structure<color>(board);
}

template<Color color>
constexpr int evaluate(Position& board) {
	const int game_phase = compute_game_phase<color>(board);

	Score us = evaluate_single_side<color>(board);
	Score them = evaluate_single_side<~color>(board);

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = us.middle_game - them.middle_game;
	const int eg_score = us.end_game - them.end_game;
	return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}