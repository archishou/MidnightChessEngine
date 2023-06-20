//
// Created by Archishmaan Peyyety on 4/16/23.
//
#include "pvs.h"
#include "../board/position.h"

PVSData data;

void reset_data() {
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

void update_best_move_results(int sub_depth, bool debug) {
	Line pv{};
	data.final_best_move = data.best_move;
	for (int i = 0; i < data.pv.length[0]; i++) {
		pv[i] = data.pv.table[0][i];
	}
	if (debug) {
		std::cout	<< "info depth " << sub_depth << " seldepth " << data.seldepth
					 <<  " score cp " << data.value << " time " << get_elapsed_time(TimeResolution::Milliseconds)
					 << " nodes " << data.nodes_searched << " pv " << pv << std::endl;
	}
}

int scale_soft_time_limit(BestMoveSearchParameters &params, PVSData& ab_results, int depth) {
	if (params.soft_time_limit == params.hard_time_limit && params.soft_time_limit == 86'400'000) return params.soft_time_limit;
	if (depth <= 7) return params.soft_time_limit;

	Move best_move = ab_results.best_move;
	double best_move_nodes_spent = static_cast<double>(ab_results.nodes_spend[best_move.from()][best_move.to()]);
	double total_nodes_spent = static_cast<double>(data.nodes_searched);
	double percent_nodes_spent = best_move_nodes_spent / total_nodes_spent;
	double scaled_soft_time_limit = (1.5 - percent_nodes_spent) * 1.35;
	return static_cast<int>(std::min(params.soft_time_limit * scaled_soft_time_limit, static_cast<double>(params.hard_time_limit)));
}
