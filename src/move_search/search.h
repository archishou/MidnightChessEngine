//
// Created by Archishmaan on 12/21/22.
//
#pragma once
#include <fstream>
#include <vector>
#include <random>
#include "move_generation/position.h"
#include "pvs.h"
#include "search_constants.h"
#include "utils/fen_constants.h"

static const BestMoveSearchParameters DEFAULT_BEST_MOVE_SEARCH_PARAMS = {
		.depth = MAX_DEPTH,
		.hard_time_limit = DEFAULT_SEARCH_TIME,
		.soft_time_limit = DEFAULT_SEARCH_TIME,
		.debug_info = true
};

void update_best_move_results(BestMoveSearchResults& search_results, PVSData& ab_results, int sub_depth, bool debug);

template<Color color>
BestMoveSearchResults iterative_deepening(Position& board, const BestMoveSearchParameters& params) {
	BestMoveSearchResults search_results;

	reset_clock();
	for (int sub_depth = 1; sub_depth <= params.depth; sub_depth++) {
		if (time_elapsed_exceeds(params.soft_time_limit, TimeResolution::Milliseconds)) {
			break;
		}
		PVSData ab_results = aspiration_windows<color>(board, search_results.value, sub_depth, params.hard_time_limit);
		if (ab_results.search_completed) {
			update_best_move_results(search_results, ab_results, sub_depth, params.debug_info);
		}
	}

	search_results.time_searched = get_elapsed_time(TimeResolution::Seconds);

	return search_results;
}

template<Color color>
BestMoveSearchResults best_move(Position& board, const BestMoveSearchParameters& parameters) {
	return iterative_deepening<color>(board, parameters);
}

template<Color color>
BestMoveSearchResults best_move(Position& board) {
	return best_move<color>(board, DEFAULT_BEST_MOVE_SEARCH_PARAMS);
}

inline BestMoveSearchResults best_move(Position& board, const BestMoveSearchParameters& parameters) {
	if (board.turn() == BLACK) return best_move<BLACK>(board, parameters);
	return best_move<WHITE>(board, parameters);
}

inline BestMoveSearchResults best_move(Position& board) {
	return best_move(board, DEFAULT_BEST_MOVE_SEARCH_PARAMS);
}
