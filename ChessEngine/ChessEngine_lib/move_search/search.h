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

template<Color color>
BestMoveSearchResults best_move(Position& board) {
	struct BestMoveSearchResults results;
	struct IDResults id_results = iterative_deepening<color>(board, MAX_TIME);
	results.best_move = id_results.best_move;
	results.depth_searched = id_results.depth_searched;
	results.time_searched = id_results.time_searched;
	results.value = id_results.value;
	return results;
}
