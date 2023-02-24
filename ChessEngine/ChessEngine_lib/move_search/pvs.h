//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "move_generation/position.h"
#include "move_search/move_ordering/move_ordering.h"
#include "pv_table.h"
#include "clock.h"

struct PVSData {
    Move best_move;
    bool search_completed{};
	int value{};
	// triangular-table-table
	PV pv;
	uint64_t nodes_searched{};
	uint64_t q_nodes_searched{};
	int seldepth{};

	int time_limit{};
};

struct MoveGenerationOptions QSearchMoveGenerationsOptions = {
	.generate_captures = true,
	.generate_checks = false,
	.generate_promotion = false,
	.generate_quiet = false,
};

static PVSData data;

void reset_data() {
	data.nodes_searched = 0;
	data.q_nodes_searched = 0;
	data.search_completed = true;
	data.seldepth = 0;
	std::memset(data.pv.table, 0, sizeof(data.pv.table));
	std::memset(data.pv.length, 0, sizeof(data.pv.length));
	data.time_limit = 0;
}

bool position_is_draw(Position &board, const int ply) {
	uint64_t current_hash = board.get_hash();
	if (board.fifty_mr_clock() >= 100) {
		return true;
	}
	int count = ply == 0 ? 0 : 1; // If it's a root node, we check for three-fold repetition. Otherwise, just two fold.
	const size_t hash_hist_size = board.hash_history.size();
	for (long idx = hash_hist_size - 3;
		 idx >= 0 && idx >= hash_hist_size - board.fifty_mr_clock();
		 idx -= 2) {
		zobrist_hash hash = board.hash_history[idx];
		if (hash == current_hash) count += 1;
		if (count >= 2) return true;
	}
	return false;
}

template<Color color>
int q_search(Position &board, const int ply, int alpha, const int beta) {

	if (time_elapsed_exceeds(data.time_limit, Milliseconds)) {
		data.search_completed = false;
		return 0;
	}

	data.seldepth = std::max(data.seldepth, ply);

	const int stand_pat = evaluate<color>(board);
	if (stand_pat >= beta) return beta;
	if (alpha < stand_pat) alpha = stand_pat;

	MoveList<color> capture_moves(board, QSearchMoveGenerationsOptions);
	ScoredMoves scored_moves = order_moves<color>(capture_moves, board, t_table, ply);
	for (const ScoredMove& scored_move : scored_moves) {
		const Move legal_move = scored_move.move;
		board.play<color>(legal_move);
		data.q_nodes_searched += 1;
		const int score = -q_search<~color>(board, ply + 1, -beta, -alpha);
		board.undo<color>(legal_move);
		if (score >= beta) return beta;
		if (score > alpha) alpha = score;
	}
	return alpha;
}

template<Color color>
int pvs(Position &board, short depth, int ply, int alpha, int beta, bool do_null) {

	if (time_elapsed_exceeds(data.time_limit, Milliseconds)) {
		data.search_completed = false;
		return 0;
	}

	int alpha_initial = alpha;
	bool in_check = board.in_check<color>();
	bool pv_node = alpha != beta - 1;

	init_pv(data.pv, ply);
	data.seldepth = std::max(data.seldepth, ply);

	if (ply > 0) {
		if (position_is_draw(board, ply)) return 0;
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) return alpha;
	}

	if (depth == 0) {
		return q_search<color>(board, ply, alpha, beta);
	}

	TranspositionTableSearchResults probe_results = t_table.probe_for_search(board.get_hash(), depth, ply);
	if (probe_results.entry_found && !pv_node) {
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

	if (depth >= 3 && !in_check && !pv_node && do_null) {
		board.play_null<color>();

		int reduction = 2 + depth/4;
		int depth_prime = std::max(depth - reduction, 0);
		int null_eval = -pvs<~color>(board, depth_prime, ply + 1, -beta, -beta + 1,
									 false);

		board.undo_null<color>();
		if (null_eval >= beta) return null_eval;
	}

	MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves<color>(all_legal_moves, board, t_table, ply);

	if (scored_moves.empty()) {
		if (in_check) return -(MATE_SCORE - ply);
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
			new_value = -pvs<~color>(board, depth - 1, ply + 1, -beta, -alpha, true);
		} else {
			new_value = -pvs<~color>(board, depth - 1, ply + 1, -alpha - 1, -alpha, true);
			if (alpha < new_value && new_value < beta) {
				new_value = -pvs<~color>(board, depth - 1, ply + 1, -beta, -alpha, true);
			}
		}
		value = std::max(value, new_value);
		board.undo<color>(legal_move);
		if (!data.search_completed) return 0;
		if (value > alpha) {
			if (ply == 0) data.value = value;
			update_pv(data.pv, ply, legal_move);
			best_move = legal_move;
		}
		alpha = std::max(alpha, value);
		if (alpha >= beta) {
			update_history<color>(best_move, depth, ply);
			break;
		}
	}

	TranspositionTableEntryNodeType node_type = t_table.get_node_type(alpha_initial, beta, value);
	t_table.put(board.get_hash(), depth, value, ply, best_move, node_type);
	if (ply != 0) {
		TranspositionTableSearchResults table_search_results = t_table.probe_for_search(board.get_hash(), depth, ply);
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
PVSData pvs_root(Position& board, short depth, int time_limit) {
	reset_data();
	data.time_limit = time_limit;
	pvs<color>(board, depth, 0, NEG_INF_CHESS, POS_INF_CHESS, false);
	data.best_move = data.pv.table[0][0];
	return data;
}