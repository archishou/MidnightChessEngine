#pragma once
#include "../../board/types/board_types.h"
#include "../search_constants.h"
#include "../../move_gen/types/move.h"
#include <vector>

enum class TTNodeType {
	EXACT,
	UPPER_NODE,
	LOWER_NODE
};

struct TranspositionTableEntry {
	ZobristHash zobrist_hash{};
	i16 depth{};
	i32 value{};
	TTNodeType node_type{};
	Move best_move{};
};

struct TranspositionTableSearchResults {
	TranspositionTableEntry entry;
	bool entry_found{};
};

constexpr short QSEARCH_TT_DEPTH = -1;
constexpr bool QSEARCH_TT_PV_NODE = false;

// More to learn from here: https://github.com/kobolabs/stockfish/blob/master/tt.cpp
class TranspositionTable {
public:
	TTNodeType get_node_type(i32 alpha_initial, i32 beta, i32 value);
	i32 correct_mate_for_storage(i32 score, i32 ply);
	i32 correct_mate_for_retrieval(i32 score, i32 ply);
	void put(ZobristHash hash, i16 depth, i32 score, i32 ply, Move best_move, bool pv_node,
			 TTNodeType node_type);
	void reset_table();
	void resize(i32 mb);
	void prefetch(ZobristHash hash);
	TranspositionTableSearchResults probe_for_move_ordering(ZobristHash hash);
	TranspositionTableSearchResults probe_for_search(ZobristHash hash, i32 depth, i32 ply);
	TranspositionTableSearchResults probe_eval(ZobristHash hash, i32 ply);
	usize mb_to_entries(i32 mb);
	explicit TranspositionTable(i32 mb = 64);
	size_t entry_count();

private:
	u64 get_index(u64 zobrist_hash);
	std::vector<TranspositionTableEntry> transposition_table;
};

extern TranspositionTable t_table;
