//
// Created by Archishmaan Peyyety on 2/14/23.
//
#pragma once
#include "move_generation/types.h"
#include "move_search/move_ordering/ordering_constants.h"
#include "move_search/search_params.h"
#include "move_search/search_constants.h"

extern int history[NSQUARES][NSQUARES];
extern Move killers[MAX_PLY][NKILLERS];

extern void init_history();
extern void scale_history();

template<Color color>
void update_history(Move &move, int depth, int ply) {
	if (move.flag() != QUIET) return;

	killers[ply][1] = killers[ply][0];
	killers[ply][0] = move;

	history[move.from()][move.to()] += depth * depth;
	if (history[move.from()][move.to()] >= -HISTORY_BONUS) {
		scale_history();
	}
}