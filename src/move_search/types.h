#pragma once
#include "move_generation/types.h"

struct ScoredMove {
	Move move;
	int score;
};

typedef std::vector<ScoredMove> ScoredMoves;
