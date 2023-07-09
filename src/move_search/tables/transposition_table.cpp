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

	PawnCacheEntry default_pawn_entry{};
	for (auto &i : pawn_cache) {
		i = default_pawn_entry;
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

u64 TranspositionTable::tt_get_index(u64 zobrist_hash) { return zobrist_hash % tt_entry_count(); }

u64 TranspositionTable::pawn_cache_get_index(u64 zobrist_hash) { return zobrist_hash % pawn_cache_entry_count(); }

TTNodeType TranspositionTable::get_node_type(i32 alpha_initial, i32 beta, i32 value) {
	TTNodeType node_type;
	if (value <= alpha_initial) node_type = UPPER_NODE;
	else if (value >= beta) node_type = LOWER_NODE;
	else node_type = EXACT;
	return node_type;
}

void TranspositionTable::put(ZobristHash hash, i16 depth, i32 score, i32 ply,
							 Move best_move, bool pv_node, TTNodeType node_type) {
	score = correct_mate_for_storage(score, ply);
	TranspositionTableEntry previous_entry = transposition_table[tt_get_index(hash)];

	if (node_type == EXACT ||
		previous_entry.zobrist_hash != hash ||
		depth + 7 + 2 * pv_node > previous_entry.depth - 4) {
		TranspositionTableEntry entry;
		entry.zobrist_hash = hash;
		entry.depth = depth;
		entry.value = score;
		entry.node_type = node_type;
		entry.best_move = best_move;
		transposition_table[tt_get_index(hash)] = entry;
	}
}

void TranspositionTable::pawn_table_put(ZobristHash hash, Score score) {
	PawnCacheEntry entry{};
	entry.score = score;
	entry.zobrist_hash = hash;
	pawn_cache[pawn_cache_get_index(hash)] = entry;
}

TranspositionTableSearchResults
TranspositionTable::probe_for_move_ordering(ZobristHash hash) {
	TranspositionTableEntry entry = transposition_table[tt_get_index(hash)];
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

	TranspositionTableEntry entry = transposition_table[tt_get_index(hash)];
	if (entry.zobrist_hash == hash && entry.depth >= depth && ply != 0) {
		results.entry_found = true;
		results.entry = entry;
		results.entry.value = correct_mate_for_retrieval(results.entry.value, ply);
	}
	return results;
}

TranspositionTableSearchResults TranspositionTable::probe_eval(ZobristHash hash, i32 ply) {
	TranspositionTableSearchResults results;
	results.entry_found = false;

	TranspositionTableEntry entry = transposition_table[tt_get_index(hash)];
	if (entry.zobrist_hash == hash) {
		results.entry_found = true;
		results.entry = entry;
		results.entry.value = correct_mate_for_retrieval(results.entry.value, ply);
	}
	return results;
}

PawnCacheProbeResults TranspositionTable::probe_pawn_cache(ZobristHash pawn_hash) {
	PawnCacheEntry entry = pawn_cache[pawn_cache_get_index(pawn_hash)];
	PawnCacheProbeResults results{};
	results.entry_found = false;
	if (entry.zobrist_hash == pawn_hash) {
		results.entry_found = true;
		results.entry = entry;
	}
	return results;
}

i32 TranspositionTable::mb_to_entries(i32 mb) {
	i32 bytes = mb * 1'048'576;
	return static_cast<i32>(bytes / sizeof(TranspositionTableEntry));
}

void TranspositionTable::resize(i32 mb) {
	i32 entries = mb_to_entries(mb);
	transposition_table.resize(entries);
	transposition_table.shrink_to_fit();
	reset_table();
}

usize TranspositionTable::tt_entry_count() {
	return transposition_table.size();
}

usize TranspositionTable::pawn_cache_entry_count() {
	return pawn_cache.size();
}

void TranspositionTable::prefetch(ZobristHash hash) {
	__builtin_prefetch(&transposition_table[tt_get_index(hash)]);
}

TranspositionTable t_table = TranspositionTable();
