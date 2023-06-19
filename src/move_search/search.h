//
// Created by Archishmaan on 12/21/22.
//
#pragma once
#include <fstream>
#include <vector>
#include <random>
#include "pvs.h"
#include "search_constants.h"

void update_best_move_results(int sub_depth, bool debug);
double scale_soft_time_limit(BestMoveSearchParameters &params, PVSData& ab_results, int depth);

template<Color color>
void iterative_deepening(Position& board, BestMoveSearchParameters& params) {
	reset_clock();
	std::memset(data.nodes_spend, 0, sizeof(data.nodes_spend));
	data.nodes_searched = 0;
	for (int sub_depth = 1; sub_depth <= params.depth; sub_depth++) {
		int soft_limit = static_cast<int>(std::min(params.soft_time_limit * scale_soft_time_limit(params, data, sub_depth), static_cast<double>(params.hard_time_limit)));
		if (time_elapsed_exceeds(soft_limit, TimeResolution::Milliseconds)) {
			break;
		}
		aspiration_windows<color>(board, data.value, sub_depth, params.hard_time_limit);
		if (data.search_completed) {
			update_best_move_results(sub_depth, params.debug_info);
		}
	}
}

inline void search(Position& board, BestMoveSearchParameters& parameters) {
	if (board.turn() == BLACK) iterative_deepening<BLACK>(board, parameters);
	else iterative_deepening<WHITE>(board, parameters);
}
