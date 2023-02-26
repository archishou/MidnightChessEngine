#include "move_generation/position.h"
#include "eval_constants.h"
#include "pawn.h"

template<Color color>
constexpr void compute_piece_score(Position& board, PieceType piece_type, Score& score, int& game_phase) {
	Bitboard piece_bitboard_us = board.bitboard_of(color, piece_type);
	while (piece_bitboard_us) {
		Square square = pop_lsb(&piece_bitboard_us);
		score += read_pstq<color>(piece_type, square) + PIECE_VALUES[piece_type];
		game_phase += GAME_PHASE_BONUS[piece_type];
	}
}

template<Color color>
constexpr int evaluate_all_piece_positions(Position& board) {
	Score us = Score(), them = Score();
	int game_phase = 0;
	for (PieceType piece_type : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
		compute_piece_score<color>(board, piece_type, us, game_phase);
		compute_piece_score<~color>(board, piece_type, them, game_phase);
	}

	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;

	const int mg_score = us.middle_game - them.middle_game;
	const int eg_score = us.end_game - them.end_game;
	return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}

template<Color color>
int evaluate(Position& board) {
	const int piece_position = evaluate_all_piece_positions<color>(board);
	const int pawn_structure = evaluate_pawn_structure<color>(board);
	return piece_position + pawn_structure;
}