//
// Created by Archishmaan Peyyety on 3/8/23.
//
#include "history_table.h"

void init_history(ThreadData &tdata) {
	for (i32 i = 0; i < NSQUARES; i++) {
		for (i32 j = 0; j < NSQUARES; j++) {
			tdata.history[WHITE][i][j] = 0;
			tdata.history[BLACK][i][j] = 0;
		}
	}

	for (i32 previous_move_ptype = 0; previous_move_ptype < NPIECES; previous_move_ptype++) {
		for (i32 previous_move_to = 0; previous_move_to < NSQUARES; previous_move_to++) {
			for (i32 move_ptype = 0; move_ptype < NPIECES; move_ptype++) {
				for (i32 move_to = 0; move_to < NSQUARES; move_to++) {
					tdata.continuation_history[previous_move_ptype][previous_move_to][move_ptype][move_to] = 0;
				}
			}
		}
	}

	for (i32 attacking = 0; attacking < NPIECES; attacking++) {
		for (i32 capture_to = 0; capture_to < NSQUARES; capture_to++) {
			for (i32 piece_attacked = 0; piece_attacked < NPIECES; piece_attacked++) {
				tdata.capture_history[attacking][capture_to][piece_attacked] = 0;
			}
		}
	}

	for (i32 i = 0; i < MAX_PLY; i++) {
		for (i32 j = 0; j < 2; j++) {
			tdata.killers[i][j] = Move();
		}
	}
}

void update_history_entry(i32& history_entry, i32 bonus) {
	history_entry -= (history_entry * abs(bonus)) / 324;
	history_entry += bonus * 32;
}

void update_killers(ThreadData &tdata, Move &best_move, i32 ply) {
	tdata.killers[ply][1] = tdata.killers[ply][0];
	tdata.killers[ply][0] = best_move;
}

void update_continuation_history(ThreadData &tdata, Position &board, Move previous_move, Move attempted_move, i32 bonus) {
	update_history_entry(
			tdata.continuation_history
			[board.piece_at(previous_move.from())][previous_move.to()]
			[board.piece_at(attempted_move.from())][attempted_move.to()],
			bonus);
}

void update_capture_history(ThreadData &tdata, Position &board, Move attempted_move, i32 bonus) {
	update_history_entry(
			tdata.capture_history
				[board.piece_at(attempted_move.from())]
				[attempted_move.to()]
				[board.piece_at(attempted_move.to())],
				bonus);
}
