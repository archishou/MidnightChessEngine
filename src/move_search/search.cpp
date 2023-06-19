//
// Created by Archishmaan Peyyety on 4/15/23.
//
#include "search.h"
#include <iostream>
#include <algorithm>

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

double scale_soft_time_limit(BestMoveSearchParameters &params, PVSData& ab_results, int depth) {
	if (params.soft_time_limit == params.hard_time_limit && params.soft_time_limit == 86'400'000) return 1.00;
	if (depth <= 7) return 1.00;

	Move best_move = ab_results.best_move;
	double percent_nodes_spent = static_cast<double>(ab_results.nodes_spend[best_move.from()][best_move.to()]) / static_cast<double>(data.nodes_searched);
	return (1.5 - percent_nodes_spent) * 1.35;
}
