//
// Created by Archishmaan Peyyety on 1/15/23.
//
#include "transposition_table.h"

TranspositionTable::TranspositionTable(uint64_t size) {
	table_size = size;
	transposition_table = new TranspositionTableEntry[table_size];
}

TranspositionTable::~TranspositionTable() {
	delete [] transposition_table;
}

void TranspositionTable::put(zobrist_hash hash, int depth, int score, TranspositionTableEntryNodeType node_type) {
	TranspositionTableEntry entry;
	entry.zobrist_hash = hash;
	entry.depth = depth;
	entry.value = score;
	entry.node_type = node_type;
	transposition_table[get_index(hash)] = entry;
}

TranspositionTableSearchResults
TranspositionTable::probe(zobrist_hash hash, int depth) {
	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	TranspositionTableSearchResults results;
	results.entry_found = false;
	if (entry.zobrist_hash == hash && entry.depth >= depth) {
		results.entry_found = true;
		results.entry = entry;
	}
	return results;
}

uint64_t TranspositionTable::get_index(uint64_t zobrist_hash) {
	return zobrist_hash % table_size;
}

TranspositionTableEntryNodeType
TranspositionTable::get_node_type(const int &alpha_initial, const int &beta, const int &value) {
	TranspositionTableEntryNodeType node_type;
	if (value <= alpha_initial) node_type = UPPER_NODE;
	else if (value >= beta) node_type = LOWER_NODE;
	else node_type = EXACT;
	return node_type;
}
