//
// Created by Archishmaan Peyyety on 1/15/23.
//
#include "transposition_table.h"

using enum TTNodeType;
TranspositionTable::TranspositionTable(i32 mb) {
	resize(mb);
}

void TranspositionTable::reset_table() {
	TranspositionTableEntry default_entry = TranspositionTableEntry();
	default_entry.value = 0;
	default_entry.zobrist_hash = 0;
	default_entry.depth = 0;
	default_entry.best_move = Move();
	default_entry.node_type = EXACT;
	for (auto & i : transposition_table) {
		i = default_entry;
	}
}

i32 TranspositionTable::correct_mate_for_retrieval(i32 score, i32 ply) {
	if (score < -MATE_BOUND) score += ply;
	else if (score > MATE_BOUND) score -= ply;
	return score;
}

i32 TranspositionTable::correct_mate_for_storage(i32 score, i32 ply) {
	if (score < -MATE_BOUND) score -= ply;
	else if (score > MATE_BOUND) score += ply;
	return score;
}

u64 TranspositionTable::get_index(u64 zobrist_hash) {
	return zobrist_hash % entry_count();
}

TTNodeType TranspositionTable::get_node_type(i32 alpha_initial, i32 beta, i32 value) {
	TTNodeType node_type;
	if (value <= alpha_initial) node_type = UPPER_NODE;
	else if (value >= beta) node_type = LOWER_NODE;
	else node_type = EXACT;
	return node_type;
}

void TranspositionTable::put(ZobristHash hash, i16 depth, i32 score, i32 ply, Move best_move, bool pv_node,
							 TTNodeType node_type) {

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
TranspositionTable::probe_for_search(ZobristHash hash, i32 depth, i32 ply) {

	TranspositionTableSearchResults results;
	results.entry_found = false;

	TranspositionTableEntry entry = transposition_table[get_index(hash)];
	if (entry.zobrist_hash == hash && entry.depth >= depth && ply != 0) {
		results.entry_found = true;
		results.entry = entry;
		results.entry.value = correct_mate_for_retrieval(results.entry.value, ply);
	}
	return results;
}

TranspositionTableSearchResults TranspositionTable::probe_eval(ZobristHash hash, i32 ply) {
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

usize TranspositionTable::mb_to_entries(i32 mb) {
	usize bytes = mb * 1'048'576;
	return static_cast<usize>(bytes / sizeof(TranspositionTableEntry));
}

void TranspositionTable::resize(i32 mb) {
	usize entries = mb_to_entries(mb);
	transposition_table.resize(entries);
	transposition_table.shrink_to_fit();
	reset_table();
}

size_t TranspositionTable::entry_count() {
	return transposition_table.size();
}

void TranspositionTable::prefetch(ZobristHash hash) {
	__builtin_prefetch(&transposition_table[get_index(hash)]);
}

TranspositionTable t_table = TranspositionTable();
