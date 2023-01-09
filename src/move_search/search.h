//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "position.h"
#include "id.h"

struct BestMoveSearchResults {
	Move bestMove;
	int depthSearched;
	double timeSearched;
};

template<Color color>
BestMoveSearchResults best_move(Position& board) {
	struct BestMoveSearchResults results;
	struct IDResults idResults = iterative_deepening<color>(board, MAX_TIME);
	results.bestMove = idResults.bestMove;
	results.depthSearched = idResults.depthSearched;
	results.timeSearched = idResults.timeSearched;
	return results;
}
