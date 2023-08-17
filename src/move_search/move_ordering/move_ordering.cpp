//
// Created by Archishmaan Peyyety on 3/20/23.
//

#include "move_ordering.h"

int get_piece_value(PieceType piece_type) {
	switch (piece_type) {
		case PAWN: return ORDERING_PAWN_VALUE;
		case KNIGHT: return ORDERING_KNIGHT_VALUE;
		case BISHOP: return ORDERING_BISHOP_VALUE;
		case ROOK: return ORDERING_ROOK_VALUE;
		case QUEEN: return ORDERING_QUEEN_VALUE;
		case KING: return ORDERING_KING_VALUE;
		default: return 0;
	}
}

int hash_move_score(Move& move, Move& previous_best_move) {
	if (move != previous_best_move) return 0;
	return PREVIOUS_BEST_MOVE_BONUS;
}

int promotion_move_score(Move move) {
	if (!move.is_promotion()) return 0;
	MoveType flag = move.type();
	if ((flag == (PR_QUEEN | CAPTURE_TYPE)) || flag == PR_QUEEN) return ORDERING_QUEEN_VALUE + PROMOTION_BONUS;
	else if ((flag == (PR_ROOK | CAPTURE_TYPE)) || flag == PR_ROOK) return ORDERING_ROOK_VALUE + PROMOTION_BONUS;
	else if ((flag == (PR_BISHOP | CAPTURE_TYPE)) || flag == PR_BISHOP) return ORDERING_BISHOP_VALUE + PROMOTION_BONUS;
	else if ((flag == (PR_KNIGHT | CAPTURE_TYPE)) || flag == PR_KNIGHT) return ORDERING_KNIGHT_VALUE + PROMOTION_BONUS;
	else return 0;
}

Move& select_move(ScoredMoves& scored_moves, int idx) {
	int best_idx = idx;
	int best_score = scored_moves[idx].score;
	for (int i = idx + 1; i < static_cast<int>(scored_moves.size()); i++) {
		if (scored_moves[i].score < best_score) {
			best_idx = i;
			best_score = scored_moves[i].score;
		}
	}
	std::swap(scored_moves[idx], scored_moves[best_idx]);
	return scored_moves[idx].move;
}
