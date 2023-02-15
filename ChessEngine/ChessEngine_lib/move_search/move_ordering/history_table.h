//
// Created by Archishmaan Peyyety on 2/14/23.
//
#include "move_generation/types.h"
#include "ordering_constants.h"

int history[NSQUARES][NSQUARES];

void init_history() {
	for (int i = 0; i < NSQUARES; i++)
		for (int j = 0; j < NSQUARES; j++)
			history[i][j] = 0;
}

void scale_history() {
	for (int i = 0; i < NSQUARES; i++)
		for (int j = 0; j < NSQUARES; j++)
			history[i][j] >>= 1;
}

void update_history(Move &move, int depth) {
	if (move.flag() != QUIET) return;
	history[move.from()][move.to()] += depth * depth;
	if (history[move.from()][move.to()] >= -HISTORY_BONUS) {
		scale_history();
	}
}