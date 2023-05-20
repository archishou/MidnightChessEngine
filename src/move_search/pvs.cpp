//
// Created by Archishmaan Peyyety on 4/16/23.
//
#include "pvs.h"
#include "../board/position.h"

PVSData data;

void reset_data() {
	data.nodes_searched = 0;
	data.q_nodes_searched = 0;
	data.search_completed = true;
	data.seldepth = 0;
	std::memset(data.pv.table, 0, sizeof(data.pv.table));
	std::memset(data.pv.length, 0, sizeof(data.pv.length));
	std::memset(data.moves_made, 0, sizeof(data.moves_made));
	std::memset(data.excluded_moves, 0, sizeof(data.excluded_moves));
	data.time_limit = 0;
}

bool position_is_draw(Position &board, const int ply) {
	if (board.fifty_move_rule() >= 100) {
		return true;
	}
	return board.has_repetition(ply == 0 ? Position::THREE_FOLD : Position::TWO_FOLD);
}
