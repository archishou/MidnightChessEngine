//
// Created by Archishmaan Peyyety on 1/8/23.
//
#include "move_generation/position.h"
#include "move_generation/tables.h"
#include "evaluation/evaluate.h"
#include "transposition_table.h"

struct ScoredMove {
	Move move;
	int score;
};

typedef std::vector<ScoredMove> ScoredMoves;

const int CAPTURED_PIECE_VALUE_MULTIPLIER = 10;
const int IN_OPP_PAWN_TERRITORY_PENALTY = -350;

bool compare_moves(ScoredMove const& lhs, ScoredMove const& rhs) {
	return lhs.score < rhs.score;
}

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

int capture_move_score(Move move, Position& board) {
	PieceType to_type = type_of(board.at(move.to()));
	PieceType from_type = type_of(board.at(move.from()));

	if (move.is_capture()) {
		return CAPTURED_PIECE_VALUE_MULTIPLIER * get_piece_value(to_type) - get_piece_value(from_type);
	}
	return 0;
}

int promotion_move_score(Move move, Position& board) {
	if (move.is_promotion()) {
		MoveFlag flag = move.flag();
		if (flag == PC_QUEEN || flag == PR_QUEEN) return QUEEN_VALUE;
		else if (flag == PC_ROOK || flag == PR_ROOK) return ROOK_VALUE;
		else if (flag == PC_BISHOP || flag == PR_BISHOP) return BISHOP_VALUE;
		else if (flag == PC_KNIGHT || flag == PR_KNIGHT) return KNIGHT_VALUE;
	}
	return 0;
}

template<Color color>
int in_opponent_pawn_territory(Move move, Position& board) {
	Bitboard opp_pawn_attacks = pawn_attacks<~color>(board.bitboard_of(~color, PAWN));
	if (move.to() & opp_pawn_attacks) return IN_OPP_PAWN_TERRITORY_PENALTY;
	return 0;
}

template<Color color>
ScoredMoves order_moves(MoveList<color>& move_list, Position& board, TranspositionTable& t_table) {
	ScoredMoves scored_moves;
	Move previous_best_move = Move();
	TranspositionTableSearchResults search_results = t_table.probe(board.get_hash());
	if (search_results.entry_found) previous_best_move = search_results.entry.best_move;
	for (Move move : move_list) {
		struct ScoredMove scored_move;
		scored_move.move = move;
		int score = 0; //Higher score is likely a better move.
		score += capture_move_score(move, board);
		score += promotion_move_score(move, board);
		score += in_opponent_pawn_territory<color>(move, board);
		if (move == previous_best_move) score += 100000;
		// Score negated for sorting. We want to evaluate high scoring moves first.
		scored_move.score = -score;
		scored_moves.push_back(scored_move);
	}
	std::sort(scored_moves.begin(), scored_moves.end(), &compare_moves);
	return scored_moves;
}

