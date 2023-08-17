//
// Created by Archishmaan Peyyety on 2/14/23.
//
#pragma once

#include "../../board/types/board_types.h"
#include "../../board/types/square.h"
#include "../../board/types/piece.h"
#include "../search_constants.h"
#include "../move_ordering/ordering_constants.h"
#include "../../board/position.h"
#include "../types.h"
#include "../../types.h"

extern void init_history(ThreadData &tdata);
extern void update_history_entry(int& history_entry, int bonus);
extern void
update_continuation_history(ThreadData &tdata, Position &board, Move previous_move, Move attempted_move, int bonus);
extern void update_capture_history(ThreadData &tdata, Position &board, Move attempted_move, int bonus);
extern void update_killers(ThreadData &tdata, Move &best_move, int ply);

template<Color color>
void
update_history(ThreadData &tdata, Position &board, ScoredMoves &ordered_moves, Move &best_move,
			   int depth, int move_idx, int ply, PVSData &data) {

	const Move one_move_ago = ply > 0 ? data.moves_made[ply - 1] : Move();
	const Move two_move_ago = ply > 1 ? data.moves_made[ply - 2] : Move();

	int history_bonus = depth * depth + depth - 1;

	if (best_move.is_quiet()) {
		update_history_entry(tdata.history[color][best_move.from()][best_move.to()], history_bonus);
		if (one_move_ago != Move())
			update_continuation_history(tdata, board, one_move_ago, best_move,
										history_bonus);
		if (two_move_ago != Move())
			update_continuation_history(tdata, board, two_move_ago, best_move,
										history_bonus);
	} else if (best_move.is_capture() && best_move.type() != ENPASSANT) {
		update_capture_history(tdata, board, best_move, history_bonus);
	}


	// We stop at idx = move_idx - 1 which is where the best move is so we don't need a condition to avoid it in the loop.
	for (int idx = 0; idx < move_idx; idx++) {
		const Move& move = ordered_moves[idx].move;
		if (move.is_quiet()) {
			update_history_entry(tdata.history[color][move.from()][move.to()], -history_bonus);
			if (one_move_ago != Move())
				update_continuation_history(tdata, board, one_move_ago, move,
											-history_bonus);
			if (two_move_ago != Move())
				update_continuation_history(tdata, board, two_move_ago, move,
											-history_bonus);
		} else if (move.is_capture() && best_move.type() != ENPASSANT) {
			update_capture_history(tdata, board, move, -history_bonus);
		}
	}
}
