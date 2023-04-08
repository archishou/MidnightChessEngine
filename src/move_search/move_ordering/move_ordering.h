//
// Created by Archishmaan Peyyety on 1/8/23.
//
#pragma once
#include <algorithm>
#include "move_generation/position.h"
#include "move_generation/tables.h"
#include "move_search/tables/transposition_table.h"
#include "move_search/tables/history_table.h"
#include "move_search/types.h"

void initialize_move_sort_tables();

int get_piece_value(PieceType piece_type);

int hash_move_score(Move& move, Move& previous_best_move);

int capture_move_score(Move move, Position& board);

int promotion_move_score(Move move, Position& board);

template<Color color>
int history_score(Move &move, int ply) {
	if (!move.is_quiet()) return 0;
	if (move == killers[ply][0]) return KILLER_MOVE_BONUS + 2000;
	else if (move == killers[ply][1]) return KILLER_MOVE_BONUS + 1000;
	return history[color][move.from()][move.to()];
}

template<Color color>
int in_opponent_pawn_territory(Move move, Position& board) {
	Bitboard opp_pawn_attacks = pawn_attacks<~color>(board.bitboard_of(~color, PAWN));
	if ((1ULL << move.to()) & opp_pawn_attacks) return IN_OPP_PAWN_TERRITORY_PENALTY;
	return 0;
}

template<Color color>
ScoredMoves order_moves(MoveList<color>& move_list, Position& board, int ply) {
	ScoredMoves scored_moves;
	scored_moves.reserve(move_list.size());
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
		score += history_score<color>(move, ply);
		score += in_opponent_pawn_territory<color>(move, board);
		// Score negated for sorting. We want to evaluate high scoring moves first.
		scored_move.score = -score;
		scored_moves.push_back(scored_move);
	}
	return scored_moves;
}

Move& select_move(ScoredMoves& scored_moves, int idx);