#include "move_generation/position.h"
#include "eval_constants.h"
#include "pawn.h"

template<Color color>
void compute_piece_score(Position& board, PieceType piece_type, int& mg_score, int& eg_score, int& game_phase) {
	Bitboard piece_bitboard_us = board.bitboard_of(color, piece_type);
	while (piece_bitboard_us) {
		Square square = pop_lsb(&piece_bitboard_us);
		mg_score += read_pstq<color>(MIDDLE, piece_type, square) + PIECE_VALUES[MIDDLE][piece_type];
		eg_score += read_pstq<color>(END, piece_type, square) + PIECE_VALUES[END][piece_type];
		game_phase += GAME_PHASE_BONUS[piece_type];
	}
}

template<Color color>
int evaluate_all_piece_positions(Position& board) {
	int mg_score_us = 0, eg_score_us = 0;
	int mg_score_them = 0, eg_score_them = 0;
	int game_phase = 0;
	for (PieceType piece_type : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
		compute_piece_score<color>(board, piece_type, mg_score_us, eg_score_us, game_phase);
		compute_piece_score<~color>(board, piece_type, mg_score_them, eg_score_them, game_phase);
	}
	int mg_score = mg_score_us - mg_score_them;
	int eg_score = eg_score_us - eg_score_them;
	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;
	return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}

template<Color color>
int evaluate(Position& board) {
	const int piece_position = evaluate_all_piece_positions<color>(board);
	const int pawn_structure = evaluate_pawn_structure<color>(board);
	return piece_position + pawn_structure;
}