//
// Created by Archishmaan Peyyety on 4/15/23.
//
#include "search.h"
#include <iostream>
#include <algorithm>

void update_best_move_results(BestMoveSearchResults& search_results, PVSData& ab_results, int sub_depth, bool debug) {
	std::memset(search_results.pv, 0, sizeof(search_results.pv));
	search_results.value = ab_results.value;
	search_results.best_move = ab_results.best_move;
	search_results.depth_searched = sub_depth;
	search_results.nodes_searched = ab_results.nodes_searched;
	search_results.seldepth = ab_results.seldepth;
	search_results.time_searched = get_elapsed_time(TimeResolution::Milliseconds);
	for (int i = 0; i < ab_results.pv.length[0]; i++) {
		search_results.pv[i] = ab_results.pv.table[0][i];
	}
	if (debug) {
		std::cout	<< "info depth " << sub_depth << " seldepth " << search_results.seldepth
					<<  " score cp " << search_results.value << " time " << search_results.time_searched
					<< " nodes " << search_results.nodes_searched << " pv " << search_results.pv << std::endl;
	}
}

double scale_soft_time_limit(BestMoveSearchParameters &params, PVSData& ab_results, int depth) {
	if (params.soft_time_limit == params.hard_time_limit && params.soft_time_limit == 86'400'000) return 1.00;
	if (depth <= 7) return 1.00;

	Move best_move = ab_results.best_move;
	double percent_nodes_spent = static_cast<double>(ab_results.nodes_spend[best_move.from()][best_move.to()]) / static_cast<double>(data.nodes_searched);
	return (1.5 - percent_nodes_spent) * 1.35;
}
