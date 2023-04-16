//
// Created by Archishmaan Peyyety on 4/16/23.
//
#include "pvs.h"

PVSData data;

void reset_data() {
	data.nodes_searched = 0;
	data.q_nodes_searched = 0;
	data.search_completed = true;
	data.seldepth = 0;
	std::memset(data.pv.table, 0, sizeof(data.pv.table));
	std::memset(data.pv.length, 0, sizeof(data.pv.length));
	data.time_limit = 0;
}

bool position_is_draw(Position &board, const int ply) {
	uint64_t current_hash = board.get_hash();
	if (board.fifty_mr_clock() >= 100) {
		return true;
	}
	int count = ply == 0 ? 0 : 1; // If it's a root node, we check for three-fold repetition. Otherwise, just two fold.
	const int hash_hist_size = static_cast<int>(board.hash_history.size());
	for (int idx = hash_hist_size - 3;
		 idx >= 0 && idx >= hash_hist_size - board.fifty_mr_clock();
		 idx -= 2) {
		ZobristHash hash = board.hash_history[idx];
		if (hash == current_hash) count += 1;
		if (count >= 2) return true;
	}
	return false;
}
