#pragma once
#include <atomic>
#include <vector>
#include "move_ordering/ordering_constants.h"
#include "tables/pv_table.h"

struct ScoredMove {
	Move move;
	i32 score{};
};

using ScoredMoves = std::vector<ScoredMove>;

struct SearchParameters {
	i16 depth = MAX_DEPTH;
	std::atomic<i32> hard_time_limit{DEFAULT_SEARCH_TIME};
	std::atomic<i32> soft_time_limit{DEFAULT_SEARCH_TIME};
	i32 thread_count = 1;
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

	i32 tidx = 0;
};

struct SearchData {
	Move best_move;
	Move final_best_move;
	bool search_completed{};
	i32 value{};
	i32 final_value{};
	// triangular-table-table
	PV pv{};
	i64 nodes_searched{};
	i32 seldepth{};

	std::atomic<i32> time_limit{0};
	i64 hard_node_limit = -1;
	i64 soft_node_limit = -1;

	SearchData& operator=(const SearchData& other) {
		best_move = other.best_move;
		final_best_move = other.final_best_move;
		search_completed = other.search_completed;
		value = other.value;
		final_value = other.final_value;

		pv = other.pv;
		nodes_searched = other.nodes_searched;
		seldepth = other.seldepth;

		time_limit = other.time_limit.load();
		hard_node_limit = other.hard_node_limit;
		soft_node_limit = other.soft_node_limit;
		return *this;
	}
};
