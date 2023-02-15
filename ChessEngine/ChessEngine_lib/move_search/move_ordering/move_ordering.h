//
// Created by Archishmaan Peyyety on 1/8/23.
//
#include "move_generation/position.h"
#include "move_generation/tables.h"
#include "evaluation/evaluate.h"
#include "move_search/transposition_table.h"
#include "history_table.h"

struct ScoredMove {
	Move move;
	int score;
};

typedef std::vector<ScoredMove> ScoredMoves;

void initialize_move_sort_tables() {
	init_history();
}

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
		case PieceType::KING: return KING_VALUE;
		default: return 0;
	}
}

int hash_move_score(Move& move, Move& previous_best_move) {
	if (move != previous_best_move) return 0;
	return PREVIOUS_BEST_MOVE_BONUS;
}

int capture_move_score(Move move, Position& board) {
	if (!move.is_capture()) return 0;
	PieceType to_type = type_of(board.at(move.to()));
	PieceType from_type = type_of(board.at(move.from()));
	return MVV_LVA_BONUS + get_piece_value(to_type) - get_piece_value(from_type);
}

int promotion_move_score(Move move, Position& board) {
	if (!move.is_promotion()) return 0;
	MoveFlag flag = move.flag();
	if (flag == PC_QUEEN || flag == PR_QUEEN) return QUEEN_VALUE + PROMOTION_BONUS;
	else if (flag == PC_ROOK || flag == PR_ROOK) return ROOK_VALUE + PROMOTION_BONUS;
	else if (flag == PC_BISHOP || flag == PR_BISHOP) return BISHOP_VALUE + PROMOTION_BONUS;
	else if (flag == PC_KNIGHT || flag == PR_KNIGHT) return KNIGHT_VALUE + PROMOTION_BONUS;
	else return 0;
}

int history_score(Move &move) {
	if (move.flag() != QUIET) return 0;
	return HISTORY_BONUS + history[move.from()][move.to()];
}

template<Color color>
int in_opponent_pawn_territory(Move move, Position& board) {
	Bitboard opp_pawn_attacks = pawn_attacks<~color>(board.bitboard_of(~color, PAWN));
	if ((1ULL << move.to()) & opp_pawn_attacks) return IN_OPP_PAWN_TERRITORY_PENALTY;
	return 0;
}

template<Color color>
ScoredMoves order_moves(MoveList<color>& move_list, Position& board, TranspositionTable& t_table) {
	ScoredMoves scored_moves;
	Move previous_best_move = Move();
	TranspositionTableSearchResults search_results = t_table.probe_for_move_ordering(board.get_hash());
	if (search_results.entry_found) previous_best_move = search_results.entry.best_move;
	for (Move move : move_list) {
		struct ScoredMove scored_move;
		scored_move.move = move;
		int score = 0; //Higher score is likely a better move.
		score += hash_move_score(move, previous_best_move);
		score += capture_move_score(move, board);
		score += promotion_move_score(move, board);
		score += history_score(move);
		score += in_opponent_pawn_territory<color>(move, board);
		// Score negated for sorting. We want to evaluate high scoring moves first.
		scored_move.score = -score;
		scored_moves.push_back(scored_move);
	}
	std::sort(scored_moves.begin(), scored_moves.end(), &compare_moves);
	return scored_moves;
}