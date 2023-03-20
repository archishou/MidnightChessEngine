//
// Created by Archishmaan Peyyety on 1/15/23.
//
#include "transposition_table.h"
#include "search_params.h"

TranspositionTable::TranspositionTable(uint64_t size) {
	table_size = size;
	transposition_table = new TranspositionTableEntry[table_size];
}

TranspositionTable::~TranspositionTable() {
	delete [] transposition_table;
}

void TranspositionTable::reset_table() {
	TranspositionTableEntry default_entry = TranspositionTableEntry();
	default_entry.value = 0;
	default_entry.zobrist_hash = 0;
	default_entry.depth = 0;
	default_entry.best_move = 0;
	default_entry.node_type = EXACT;
	for (int i = 0; i < table_size; i++) {
		transposition_table[i] = default_entry;
	}
}

int TranspositionTable::correct_mate_for_retrieval(int score, int ply) {
	if (score < -MATE_BOUND) score += ply;
	else if (score > MATE_BOUND) score -= ply;
	return score;
}

int TranspositionTable::correct_mate_for_storage(int score, int ply) {
	if (score < -MATE_BOUND) score -= ply;
	else if (score > MATE_BOUND) score += ply;
	return score;
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

void TranspositionTable::put(zobrist_hash hash, short depth, int score, int ply, Move best_move, bool pv_node,
							 TranspositionTableEntryNodeType node_type) {

	score = correct_mate_for_storage(score, ply);
	TranspositionTableEntry previous_entry = transposition_table[get_index(hash)];

	if (node_type == EXACT ||
		previous_entry.zobrist_hash != hash ||
		depth + 7 + 2 * pv_node > previous_entry.depth - 4) {
		TranspositionTableEntry entry;
		entry.zobrist_hash = hash;
		entry.depth = depth;
		entry.value = score;
		entry.node_type = node_type;
		entry.best_move = best_move;
		transposition_table[get_index(hash)] = entry;
	}
}

TranspositionTableSearchResults
TranspositionTable::probe_for_move_ordering(zobrist_hash hash) {
	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	TranspositionTableSearchResults results;
	results.entry_found = false;
	if (entry.zobrist_hash == hash) {
		results.entry_found = true;
		results.entry = entry;
	}
	return results;
}

TranspositionTableSearchResults
TranspositionTable::probe_for_search(zobrist_hash hash, int depth, int ply) {
	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	TranspositionTableSearchResults results;
	results.entry_found = false;
	if (entry.zobrist_hash == hash && entry.depth >= depth && ply != 0) {
		results.entry_found = true;
		results.entry = entry;
		results.entry.value = correct_mate_for_retrieval(results.entry.value, ply);
	}
	return results;
}

TranspositionTableSearchResults TranspositionTable::probe_eval(zobrist_hash hash, int ply) {
	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	TranspositionTableSearchResults results;
	results.entry_found = false;
	if (entry.zobrist_hash == hash) {
		results.entry_found = true;
		results.entry = entry;
		results.entry.value = correct_mate_for_retrieval(results.entry.value, ply);
	}
	return results;
}

bool TranspositionTable::key_in_table(zobrist_hash hash) {
	return transposition_table[get_index(hash)].zobrist_hash == 0;
}

int TranspositionTable::count_entries() {
	int entries = 0;
	for (int i = 0; i < table_size; i++) {
		if (transposition_table[i].zobrist_hash != 0) {
			entries += 1;
		}
	}
	return entries;
}
