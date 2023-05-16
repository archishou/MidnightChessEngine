#pragma once
#include "search_constants.h"
#include "move_search/tables/pv_table.h"
#include "move_gen/types/move.h"

struct ScoredMove {
	Move move;
	int score{};
};

typedef std::vector<ScoredMove> ScoredMoves;

struct BestMoveSearchParameters {
	short depth = MAX_DEPTH;
	int hard_time_limit = DEFAULT_SEARCH_TIME;
	int soft_time_limit = DEFAULT_SEARCH_TIME;
	bool debug_info = false;
};

typedef Move Line[MAX_DEPTH];
extern std::ostream& operator<<(std::ostream& os, const Line& line);

struct BestMoveSearchResults {
	Move best_move = Move();
	Line pv;
	int depth_searched = 0;
	int seldepth = 0;

	int time_searched = 0;
	int value = 0;

	uint64_t q_nodes_searched = 0;
	uint64_t nodes_searched = 0;
};

extern std::ostream& operator<<(std::ostream& os, const BestMoveSearchResults& results);

struct PVSData {
	Move best_move;
	bool search_completed{};
	int value{};
	// triangular-table-table
	PV pv{};
	uint64_t nodes_searched{};
	uint64_t q_nodes_searched{};
	int seldepth{};

	Move moves_made[MAX_PLY];
	int time_limit{};

	Move excluded_moves[MAX_PLY]{};
};
