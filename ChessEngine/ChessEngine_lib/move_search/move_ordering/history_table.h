//
// Created by Archishmaan Peyyety on 2/14/23.
//
#include "move_generation/types.h"
#include "ordering_constants.h"

int history[NSQUARES][NSQUARES];
Move killers[MAX_PLY][NKILLERS];

void init_history() {
	for (int i = 0; i < NSQUARES; i++)
		for (int j = 0; j < NSQUARES; j++)
			history[i][j] = 0;

	for (int i = 0; i < MAX_PLY; i++) {
		for (int j = 0; j < NKILLERS; j++) {
			killers[i][j] = Move();
		}
	}
}

void scale_history() {
	for (int i = 0; i < NSQUARES; i++)
		for (int j = 0; j < NSQUARES; j++)
			history[i][j] >>= 1;
}

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