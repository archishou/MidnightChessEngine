#pragma once
#include "misc.h"
#include "psts.h"

// If this is updated be sure compute_piece_count_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * (NPIECE_TYPES - 1) + attacked];
}

template<Color color, PieceType piece_type>
constexpr Score read_pst(Square square) {
	if constexpr (color == WHITE) square = flip(square);
	switch (piece_type) {
		case PAWN: return PAWN_TABLE[square];
		case KNIGHT: return KNIGHT_TABLE[square];
		case BISHOP: return BISHOP_TABLE[square];
		case ROOK: return ROOK_TABLE[square];
		case QUEEN: return QUEEN_TABLE[square];
		case KING: return KING_TABLE[square];
		default: return {};
	}
}