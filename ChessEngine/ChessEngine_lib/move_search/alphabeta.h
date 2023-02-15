//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "move_generation/position.h"
#include "move_ordering.h"
#include "pv_table.h"
#include "clock.h"

struct AlphaBetaData {
    Move best_move;
    bool search_completed;
	int value;
	// triangular-table-table
	PV pv;
	uint64_t nodes_searched;
	uint64_t q_nodes_searched;
	int seldepth;
	int tt_key_collisions;
	int nodes_in_transposition_table;
};

struct MoveGenerationOptions QSearchMoveGenerationsOptions = {
	.generate_captures = true,
	.generate_checks = false,
	.generate_promotion = false,
	.generate_quiet = false,
};

bool position_is_draw(Position &board) {
	uint64_t current_hash = board.get_hash();
	if (board.fifty_mr_clock() >= 100) {
		return true;
	}
	int count = 0;
	for (uint64_t hash : board.hash_history) {
		if (hash == current_hash) count += 1;
		if (count >= 3) return true;
	}
	return false;
}

template<Color color>
int q_search(Position &board, const int ply, int alpha, const int beta, AlphaBetaData &data, const int time_limit, TranspositionTable &t_table) {

	if (time_elapsed_exceeds(time_limit, Milliseconds)) {
		data.search_completed = false;
		return 0;
	}

	data.seldepth = std::max(data.seldepth, ply);

	const int stand_pat = evaluate<color>(board);
	if (stand_pat >= beta) return beta;
	if (alpha < stand_pat) alpha = stand_pat;

	MoveList<color> capture_moves(board, QSearchMoveGenerationsOptions);
	ScoredMoves scored_moves = order_moves<color>(capture_moves, board, t_table);
	for (const ScoredMove& scored_move : scored_moves) {
		const Move legal_move = scored_move.move;
		board.play<color>(legal_move);
		const int score = -q_search<~color>(board, ply + 1, -beta, -alpha, data, time_limit, t_table);
		board.undo<color>(legal_move);
		data.q_nodes_searched += 1;
		if (score >= beta) return beta;
		if (score > alpha) alpha = score;
	}
	return alpha;
}

template<Color color>
int alpha_beta(Position& board, short depth, int ply, int alpha, int beta, bool is_pv, AlphaBetaData& data, const int time_limit, TranspositionTable& t_table) {

	if (time_elapsed_exceeds(time_limit, Milliseconds)) {
		data.search_completed = false;
		return 0;
	}

	data.seldepth = std::max(data.seldepth, ply);
	int alpha_initial = alpha;
	init_pv(data.pv, ply);

	if (ply > 0) {
		if (position_is_draw(board)) return 0;
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) return alpha;
	}

	if (depth == 0) {
		return q_search<color>(board, ply, alpha, beta, data, time_limit, t_table);
	}

	TranspositionTableSearchResults probe_results = t_table.probe_for_search(board.get_hash(), depth, ply, is_pv);
	if (probe_results.entry_found) {
		TranspositionTableEntry tt_entry = probe_results.entry;
		if (tt_entry.node_type == EXACT) {
			return tt_entry.value;
		} else if (tt_entry.node_type == LOWER_NODE) {
			alpha = std::max(alpha, tt_entry.value);
		} else if (tt_entry.node_type == UPPER_NODE) {
			beta = std::min(beta, tt_entry.value);
		}
		if (alpha >= beta) {
			return tt_entry.value;
		}
	}

	MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board, t_table);

	if (scored_moves.empty()) {
		if (board.in_check<color>()) return -(MATE_SCORE - ply);
		return 0;
	}

	Move best_move = scored_moves.begin()->move;
	int value = NEG_INF_CHESS;
	for (int i = 0; i < scored_moves.size(); i++) {
		Move legal_move = scored_moves[i].move;
		board.play<color>(legal_move);
		data.nodes_searched += 1;
		int new_value;
		if (i == 0) {
			new_value = -alpha_beta<~color>(board, depth - 1, ply + 1, -beta, -alpha, is_pv,data, time_limit, t_table);
		} else {
			new_value =-alpha_beta<~color>(board, depth - 1, ply + 1, -beta, -alpha, false,data, time_limit, t_table);
			if (alpha < new_value && new_value < beta) {
				new_value =-alpha_beta<~color>(board, depth - 1, ply + 1, -beta, -alpha, true,data, time_limit, t_table);
			}
		}
		value = std::max(value, new_value);
		board.undo<color>(legal_move);
		if (value > alpha) {
			if (ply == 0) data.value = value;
			update_pv(data.pv, ply, legal_move);
			best_move = legal_move;
		}
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}

	TranspositionTableEntryNodeType node_type = t_table.get_node_type(alpha_initial, beta, value);
	t_table.put(board.get_hash(), depth, value, ply, best_move, node_type);
	if (!is_pv) {
		TranspositionTableSearchResults table_search_results = t_table.probe_for_search(board.get_hash(), depth, ply, is_pv);
		assert(table_search_results.entry_found);
		assert(table_search_results.entry.value == value);
		assert(table_search_results.entry.zobrist_hash == board.get_hash());
		assert(table_search_results.entry.depth == depth);
		assert(table_search_results.entry.best_move == best_move);
		assert(table_search_results.entry.node_type == node_type);
	}
	return value;
}

template<Color color>
AlphaBetaData alpha_beta_root(Position& board, short depth, int time_limit, TranspositionTable& t_table) {
	AlphaBetaData data;
	data.nodes_searched = 0;
	data.q_nodes_searched = 0;
	data.search_completed = true;
	data.seldepth = 0;
	data.tt_key_collisions = 0;
	data.nodes_in_transposition_table = 0;
	std::memset(data.pv.table, 0, sizeof(data.pv.table));
	std::memset(data.pv.length, 0, sizeof(data.pv.length));
	alpha_beta<color>(board, depth, 0, NEG_INF_CHESS, POS_INF_CHESS, true,data, time_limit, t_table);
	data.best_move = data.pv.table[0][0];
	return data;
}