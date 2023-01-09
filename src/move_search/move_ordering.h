//
// Created by Archishmaan Peyyety on 1/8/23.
//
#include "position.h"
#include "evaluate.h"

struct ScoredMove {
	Move move;
	int score;
};

typedef std::vector<ScoredMove> ScoredMoves;

const int CAPTURED_PIECE_VALUE_MULTIPLIER = 10;

int get_piece_value(PieceType piece_type) {
	switch (piece_type) {
		case PieceType::PAWN: return PAWN_VALUE;
		case PieceType::KNIGHT: return KNIGHT_VALUE;
		case PieceType::BISHOP: return BISHOP_VALUE;
		case PieceType::ROOK: return ROOK_VALUE;
		case PieceType::QUEEN: return QUEEN_VALUE;
		default: return 0;
	}
}

bool compare_moves(ScoredMove const& lhs, ScoredMove const& rhs) {
	return lhs.score < rhs.score;
}

template<Color color>
ScoredMoves order_moves(MoveList<color>& move_list, Position& board) {
	ScoredMoves scored_moves;
	for (Move move : move_list) {
		struct ScoredMove scored_move;
		scored_move.move = move;
		int score = 0; //Higher score is likely a better move.
		PieceType to_type = type_of(board.at(move.to()));
		PieceType from_type = type_of(board.at(move.from()));

		if (move.is_capture()) {
			score += CAPTURED_PIECE_VALUE_MULTIPLIER * get_piece_value(to_type) - get_piece_value(from_type);
		}

		if (move.is_promotion()) {
			MoveFlag flag = move.flag();
			if (flag == PC_QUEEN || flag == PR_QUEEN) score += QUEEN_VALUE;
			else if (flag == PC_ROOK || flag == PR_ROOK) score += ROOK_VALUE;
			else if (flag == PC_BISHOP || flag == PR_BISHOP) score += BISHOP_VALUE;
			else if (flag == PC_KNIGHT || flag == PR_KNIGHT) score += KNIGHT_VALUE;
		}

		// Score negated for sorting. We want to evaluate high scoring moves first.
		scored_move.score = -score;
		scored_moves.push_back(scored_move);
	}
	std::sort(scored_moves.begin(), scored_moves.end(), &compare_moves);
	return scored_moves;
}