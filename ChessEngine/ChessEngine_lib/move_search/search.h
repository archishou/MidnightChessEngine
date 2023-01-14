//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "move_generation/position.h"
#include "id.h"

struct BestMoveSearchResults {
	Move best_move;
	int depth_searched;
	double time_searched;
	int value;
};

struct BestMoveSearchParameters {
	int depth;
	int time_limit;
};

const struct BestMoveSearchParameters DEFAULT_BEST_MOVE_SEARCH_PARAMS = {
	.depth = 6,
	.time_limit = 1000
};

template<Color color>
BestMoveSearchResults best_move(Position& board) {
	return best_move<color>(board, DEFAULT_BEST_MOVE_SEARCH_PARAMS);
}

template<Color color>
BestMoveSearchResults best_move(Position& board, const BestMoveSearchParameters& parameters) {
	struct BestMoveSearchResults results;
	struct IDResults id_results = iterative_deepening<color>(board,
			parameters.time_limit, parameters.depth);
	results.best_move = id_results.best_move;
	results.depth_searched = id_results.depth_searched;
	results.time_searched = id_results.time_searched;
	results.value = id_results.value;
	return results;
}
