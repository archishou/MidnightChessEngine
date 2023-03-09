//
// Created by Archishmaan Peyyety on 3/8/23.
//
#include "move_search/search_params.h"
#include "history_table.h"

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

