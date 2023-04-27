//
// Created by Archishmaan Peyyety on 3/8/23.
//
#include "history_table.h"

int history[NCOLORS][NSQUARES][NSQUARES];
int continuation_history[NPIECES][NSQUARES][NPIECES][NSQUARES];
Move killers[MAX_PLY][NKILLERS];

void init_history() {
	for (int i = 0; i < NSQUARES; i++) {
		for (int j = 0; j < NSQUARES; j++) {
			history[WHITE][i][j] = 0;
			history[BLACK][i][j] = 0;
		}
	}

	for (int previous_move_ptype = 0; previous_move_ptype < NPIECE_TYPES; previous_move_ptype++) {
		for (int previous_move_to = 0; previous_move_to < NSQUARES; previous_move_to++) {
			for (int move_ptype = 0; move_ptype < NPIECE_TYPES; move_ptype++) {
				for (int move_to = 0; move_to < NSQUARES; move_to++) {
					continuation_history[previous_move_ptype][previous_move_to][move_ptype][move_to] = 0;
				}
			}
		}
	}

	for (int i = 0; i < MAX_PLY; i++) {
		for (int j = 0; j < NKILLERS; j++) {
			killers[i][j] = Move();
		}
	}
}

void update_history_entry(int& history_entry, int bonus) {
	history_entry -= (history_entry * abs(bonus)) / 324;
	history_entry += bonus * 32;
}

void update_killers(Move &best_move, int ply) {
	killers[ply][1] = killers[ply][0];
	killers[ply][0] = best_move;
}

void update_continuation_history(Position &board, Move previous_move, Move attempted_move, int bonus) {
	update_history_entry(
			continuation_history
			[board.at(previous_move.from())][previous_move.to()]
			[board.at(attempted_move.from())][attempted_move.to()],
			bonus);
}
