#pragma once
#include "search_constants.h"
#include "../move_gen/types/move.h"
#include "tables/pv_table.h"
#include <vector>

struct ScoredMove {
	Move move;
	int score{};
};

using ScoredMoves = std::vector<ScoredMove>;

struct BestMoveSearchParameters {
	short depth = MAX_DEPTH;
	int hard_time_limit = DEFAULT_SEARCH_TIME;
	int soft_time_limit = DEFAULT_SEARCH_TIME;
	bool debug_info = false;
};

typedef Move Line[MAX_DEPTH];
extern std::ostream& operator<<(std::ostream& os, const Line& line);

struct PVSData {
	Move best_move;
	Move final_best_move;
	bool search_completed{};
	int value{};
	// triangular-table-table
	PV pv{};
	uint64_t nodes_searched{};
	int seldepth{};

	Move moves_made[MAX_PLY];
	int time_limit{};

	Move excluded_moves[MAX_PLY]{};
	uint64_t nodes_spend[NSQUARES][NSQUARES]{};
	int evals[MAX_PLY]{};
};
