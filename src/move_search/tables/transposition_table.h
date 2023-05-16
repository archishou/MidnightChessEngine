#pragma once
#include "board/types/board_types.h"
#include "move_gen/types/move.h"

enum TranspositionTableEntryNodeType {
	EXACT,
	UPPER_NODE,
	LOWER_NODE
};

struct TranspositionTableEntry {
	ZobristHash zobrist_hash;
	short depth;
	int value;
	TranspositionTableEntryNodeType node_type;
	Move best_move;
};

struct TranspositionTableSearchResults {
	TranspositionTableEntry entry;
	bool entry_found;
};

constexpr short QSEARCH_TT_DEPTH = -1;
constexpr bool QSEARCH_TT_PV_NODE = false;

// More to learn from here: https://github.com/kobolabs/stockfish/blob/master/tt.cpp
class TranspositionTable {
public:
	TranspositionTableEntryNodeType get_node_type(const int &alpha_initial, const int &beta, const int &value);
	int correct_mate_for_storage(int score, int ply);
	int correct_mate_for_retrieval(int score, int ply);
	void put(ZobristHash hash, short depth, int score, int ply, Move best_move, bool pv_node,
			 TranspositionTableEntryNodeType node_type);
	void reset_table();
	void resize(int mb);
	void prefetch(ZobristHash hash);
	TranspositionTableSearchResults probe_for_move_ordering(ZobristHash hash);
	TranspositionTableSearchResults probe_for_search(ZobristHash hash, int depth, int ply);
	TranspositionTableSearchResults probe_eval(ZobristHash hash, int ply);
	int mb_to_entries(int mb);
	explicit TranspositionTable(int mb = 64);
	size_t entry_count();

private:
	uint64_t get_index(uint64_t zobrist_hash);
	std::vector<TranspositionTableEntry> transposition_table;
};

extern TranspositionTable t_table;
