#pragma once
#include "move_ordering/ordering_constants.h"
#include "tables/pv_table.h"

struct ScoredMove {
	Move move;
	i32 score{};
};

using ScoredMoves = std::vector<ScoredMove>;

struct SearchParameters {
	i16 depth = MAX_DEPTH;
	i64 node_limit = -1;
	i32 hard_time_limit = DEFAULT_SEARCH_TIME;
	i32 soft_time_limit = DEFAULT_SEARCH_TIME;
	bool debug_info = false;
};

using Line = Move[MAX_DEPTH];
extern std::ostream& operator<<(std::ostream& os, const Line& line);

struct SearchStack {
	Move move{};
	Move excluded_move{};
	i32 static_eval{};
};

struct ThreadData {
	array<array<array<i32, NSQUARES>, NSQUARES>, NCOLORS> history{};
	array<array<array<array<i32, NSQUARES>, NPIECES>, NSQUARES>, NPIECES> continuation_history{};
	array<array<array<i32, NPIECES>, NSQUARES>, NPIECES> capture_history{};
	array<array<Move, 2>, MAX_PLY> killers{};

	array<SearchStack, MAX_PLY> thread_stack{};
};

struct SearchData {
	Move best_move;
	Move final_best_move;
	bool search_completed{};
	i32 value{};
	i32 final_value{};
	// triangular-table-table
	PV pv{};
	u64 nodes_searched{};
	i32 seldepth{};

	i32 time_limit{};
	i64 hard_node_limit = -1;
	i64 soft_node_limit = -1;
};
