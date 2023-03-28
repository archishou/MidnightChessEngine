//
// Created by Archishmaan Peyyety on 2/14/23.
//
#pragma once
#include "move_generation/types.h"
#include "move_search/move_ordering/ordering_constants.h"
#include "move_search/search_params.h"
#include "move_search/search_constants.h"
#include "array"
#include "move_search/types.h"

typedef std::array<Move, MAX_PLY> MoveHistory;

extern int history[NCOLORS][NSQUARES][NSQUARES];
extern Move killers[MAX_PLY][NKILLERS];

extern void init_history();
extern void scale_history();
extern void update_history_entry(int& history_entry, int bonus);

template<Color color>
void update_history(ScoredMoves& ordered_moves, Move &best_move, int depth, int ply, int move_idx) {
	if (best_move.flag() != QUIET) return;

	killers[ply][1] = killers[ply][0];
	killers[ply][0] = best_move;

	int history_bonus = depth * depth + depth - 1;

	update_history_entry(history[color][best_move.from()][best_move.to()], history_bonus);

	for (int idx = 0; idx < move_idx; idx++) {
		const Move& move = ordered_moves[idx].move;
		if (move.flag() != QUIET) continue;
		update_history_entry(history[color][move.from()][move.to()], -history_bonus);
	}

	/*
	if (history[color][best_move.from()][best_move.to()] >= HISTORY_BONUS_CAP) {
		scale_history();
	}
	 */
}