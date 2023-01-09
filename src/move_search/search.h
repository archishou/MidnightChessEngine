//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "position.h"
#include "id.h"

struct BestMoveSearchResults {
	Move best_move;
	int depth_searched;
	double time_searched;
};

template<Color color>
BestMoveSearchResults best_move(Position& board) {
	struct BestMoveSearchResults results;
	struct IDResults idResults = iterative_deepening<color>(board, MAX_TIME);
	results.best_move = idResults.best_move;
	results.depth_searched = idResults.depth_searched;
	results.time_searched = idResults.time_searched;
	return results;
}
