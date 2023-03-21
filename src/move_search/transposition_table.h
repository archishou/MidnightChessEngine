#pragma once
#include "move_generation/position.h"
enum TranspositionTableEntryNodeType {
	EXACT,
	UPPER_NODE,
	LOWER_NODE
};

enum TranspositionTableEntrySearchType {
	Q_SEARCH,
	FULL_SEARCH
};

struct TranspositionTableEntry {
	zobrist_hash zobrist_hash;
	short depth;
	int value;
	TranspositionTableEntryNodeType node_type;
	Move best_move;
};

struct TranspositionTableSearchResults {
	TranspositionTableEntry entry;
	bool entry_found;
};

// More to learn from here: https://github.com/kobolabs/stockfish/blob/master/tt.cpp
class TranspositionTable {
public:
	TranspositionTableEntryNodeType get_node_type(const int &alpha_initial, const int &beta, const int &value);
	int correct_mate_for_storage(int score, int ply);
	int correct_mate_for_retrieval(int score, int ply);
	void put(zobrist_hash hash, short depth, int score, int ply, Move best_move, bool pv_node,
			 TranspositionTableEntryNodeType node_type);
	void reset_table();
	TranspositionTableSearchResults probe_for_move_ordering(zobrist_hash hash);
	TranspositionTableSearchResults probe_for_search(zobrist_hash hash, int depth, int ply);
	TranspositionTableSearchResults probe_eval(zobrist_hash hash, int ply);
	explicit TranspositionTable(uint64_t size = 3'000'000);
	~TranspositionTable();

private:
	uint64_t get_index(uint64_t zobrist_hash);
	TranspositionTableEntry *transposition_table;
	uint64_t table_size;
};

static TranspositionTable t_table = TranspositionTable();
