//
// Created by Archishmaan Peyyety on 1/15/23.
//
#include "transposition_table.h"
#include "move_search/search_params.h"
#include "move_search/search_constants.h"

TranspositionTable::TranspositionTable(int mb) {
	resize(mb);
}

void TranspositionTable::reset_table() {
	TranspositionTableEntry default_entry = TranspositionTableEntry();
	default_entry.value = 0;
	default_entry.zobrist_hash = 0;
	default_entry.depth = 0;
	default_entry.best_move = 0;
	default_entry.node_type = EXACT;
	for (int i = 0; i < transposition_table.size(); i++) {
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
	return zobrist_hash % entry_count();
}

TranspositionTableEntryNodeType
TranspositionTable::get_node_type(const int &alpha_initial, const int &beta, const int &value) {
	TranspositionTableEntryNodeType node_type;
	if (value <= alpha_initial) node_type = UPPER_NODE;
	else if (value >= beta) node_type = LOWER_NODE;
	else node_type = EXACT;
	return node_type;
}

void TranspositionTable::put(ZobristHash hash, short depth, int score, int ply, Move best_move, bool pv_node,
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
TranspositionTable::probe_for_move_ordering(ZobristHash hash) {
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
TranspositionTable::probe_for_search(ZobristHash hash, int depth, int ply) {
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

TranspositionTableSearchResults TranspositionTable::probe_eval(ZobristHash hash, int ply) {
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

int TranspositionTable::mb_to_entries(int mb) {
	int bytes = mb * 1'048'576;
	return static_cast<int>(bytes / sizeof(TranspositionTableEntry));
}

void TranspositionTable::resize(int mb) {
	int entries = mb_to_entries(mb);
	transposition_table.resize(entries);
	transposition_table.shrink_to_fit();
	reset_table();
}

size_t TranspositionTable::entry_count() {
	return transposition_table.size();
}

TranspositionTable t_table = TranspositionTable();
