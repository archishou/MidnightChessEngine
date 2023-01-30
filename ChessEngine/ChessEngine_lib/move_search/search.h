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
	Line pv;
	int depth_searched;
	double time_searched;
	int value;

	uint64_t nodes_searched;
	double nodes_per_second;
	int seldepth;
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

	results.nodes_searched = id_results.nodes_searched;
	results.nodes_per_second = id_results.nodes_per_second;
	results.seldepth = id_results.seldepth;
	std::copy(std::begin(id_results.pv), std::end(id_results.pv), std::begin(results.pv));
	return results;
}
