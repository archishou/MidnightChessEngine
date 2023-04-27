//
// Created by Archishmaan Peyyety on 2/14/23.
//
#pragma once
#include "move_generation/types.h"
#include "move_generation/position.h"
#include "move_search/move_ordering/ordering_constants.h"
#include "move_search/search_params.h"
#include "move_search/search_constants.h"
#include "array"
#include "move_search/types.h"

extern int history[NCOLORS][NSQUARES][NSQUARES];
extern int continuation_history[NPIECES][NSQUARES][NPIECES][NSQUARES];
extern Move killers[MAX_PLY][NKILLERS];

extern void init_history();
extern void update_history_entry(int& history_entry, int bonus);
extern void update_continuation_history(Position& board, Move previous_move, Move attempted_move, int bonus);
extern void update_killers(Move &best_move, int ply);

template<Color color>
void update_history(Position& board, ScoredMoves& ordered_moves, Move &best_move, int depth, int move_idx, int ply, PVSData& data) {
	if (!best_move.is_quiet()) return;

	const Move one_move_ago = ply > 0 ? data.moves_made[ply - 1] : Move();
	const Move two_move_ago = ply > 1 ? data.moves_made[ply - 2] : Move();

	int history_bonus = depth * depth + depth - 1;

	update_history_entry(history[color][best_move.from()][best_move.to()], history_bonus);

	if (one_move_ago != Move()) update_continuation_history(board, one_move_ago, best_move, history_bonus);
	if (two_move_ago != Move()) update_continuation_history(board, two_move_ago, best_move, history_bonus);

	// We stop at idx = move_idx - 1 which is where the best move is so we don't need a condition to avoid it in the loop.
	for (int idx = 0; idx < move_idx; idx++) {
		const Move& move = ordered_moves[idx].move;
		if (!move.is_quiet()) continue;
		update_history_entry(history[color][move.from()][move.to()], -history_bonus);

		if (one_move_ago != Move()) update_continuation_history(board, one_move_ago, move, -history_bonus);
		if (two_move_ago != Move()) update_continuation_history(board, two_move_ago, move, -history_bonus);
	}
}
