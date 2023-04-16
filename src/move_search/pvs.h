//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once

#include <array>
#include "cstring"
#include "search_params.h"
#include "move_generation/position.h"
#include "move_search/move_ordering/move_ordering.h"
#include "evaluation/evaluate.h"
#include "move_search/tables/pv_table.h"
#include "utils/clock.h"
#include "move_search/tables/lmr_table.h"
#include "reductions.h"

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

inline const MoveGenerationOptions QSearchMoveGenerationsOptions = {
	.generate_captures = true,
	.generate_checks = false,
	.generate_promotion = false,
	.generate_quiet = false,
};

static PVSData data;

inline void reset_data() {
	data.nodes_searched = 0;
	data.q_nodes_searched = 0;
	data.search_completed = true;
	data.seldepth = 0;
	std::memset(data.pv.table, 0, sizeof(data.pv.table));
	std::memset(data.pv.length, 0, sizeof(data.pv.length));
	data.time_limit = 0;
}

inline bool position_is_draw(Position &board, const int ply) {
	uint64_t current_hash = board.get_hash();
	if (board.fifty_mr_clock() >= 100) {
		return true;
	}
	int count = ply == 0 ? 0 : 1; // If it's a root node, we check for three-fold repetition. Otherwise, just two fold.
	const int hash_hist_size = static_cast<int>(board.hash_history.size());
	for (int idx = hash_hist_size - 3;
		 idx >= 0 && idx >= hash_hist_size - board.fifty_mr_clock();
		 idx -= 2) {
		ZobristHash hash = board.hash_history[idx];
		if (hash == current_hash) count += 1;
		if (count >= 2) return true;
	}
	return false;
}

template<Color color>
int q_search(Position &board, const int ply, int alpha, const int beta) {

	if (ply >= MAX_PLY - 2) return evaluate<color>(board);

	if ((data.q_nodes_searched + data.nodes_searched) % 1024 == 0) {
		if (time_elapsed_exceeds(data.time_limit, TimeResolution::Milliseconds)) {
			data.search_completed = false;
			return 0;
		}
	}

	data.seldepth = std::max(data.seldepth, ply);

	const int stand_pat = evaluate<color>(board);
	if (stand_pat >= beta) return beta;
	if (alpha < stand_pat) alpha = stand_pat;

	TranspositionTableSearchResults probe_results = t_table.probe_for_search(board.get_hash(), QSEARCH_TT_DEPTH, ply);
	if (probe_results.entry_found) {
		if ((probe_results.entry.node_type == EXACT) ||
			(probe_results.entry.node_type == LOWER_NODE && probe_results.entry.value >= beta) ||
			(probe_results.entry.node_type == UPPER_NODE && probe_results.entry.value <= alpha)) {
			return probe_results.entry.value;
		}
	}

	Move best_move = Move();
	MoveList<color> capture_moves(board, QSearchMoveGenerationsOptions);
	ScoredMoves scored_moves = order_moves<color>(capture_moves, board, ply);
	for (int move_idx = 0; move_idx < static_cast<int>(scored_moves.size()); move_idx++) {
		const Move legal_move = select_move(scored_moves, move_idx);
		board.play<color>(legal_move);
		data.q_nodes_searched += 1;
		const int score = -q_search<~color>(board, ply + 1, -beta, -alpha);
		board.undo<color>(legal_move);
		if (score >= beta) {
			alpha = beta;
			best_move = legal_move;
			break;
		}
		if (score > alpha) {
			best_move = legal_move;
			alpha = score;
		}
	}
	TranspositionTableEntryNodeType node_type;
	if (alpha >= beta) node_type = LOWER_NODE;
	else node_type = UPPER_NODE;
	t_table.put(board.get_hash(), QSEARCH_TT_DEPTH, alpha, ply, best_move, QSEARCH_TT_PV_NODE, node_type);
	return alpha;
}

template<Color color>
int pvs(Position &board, short depth, int ply, int alpha, int beta, bool do_null) {
	if (ply >= MAX_PLY - 2) return evaluate<color>(board);

	if ((data.q_nodes_searched + data.nodes_searched) % 1024 == 0) {
		if (time_elapsed_exceeds(data.time_limit, TimeResolution::Milliseconds)) {
			data.search_completed = false;
			return 0;
		}
	}

	init_pv(data.pv, ply);
	data.seldepth = std::max(data.seldepth, ply);

	if (ply > 0) {
		if (position_is_draw(board, ply)) return 0;
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) return alpha;
	}

	int alpha_initial = alpha;
	bool in_check = board.in_check<color>();
	bool pv_node = alpha != beta - 1;
	int static_eval = 0;

	if (in_check) depth++;

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

	probe_results = t_table.probe_eval(board.get_hash(), ply);
	if (probe_results.entry_found) {
		static_eval = probe_results.entry.value;
	} else {
		static_eval = evaluate<color>(board);
	}

	if (depth >= NMP_MIN_DEPTH && !in_check && !pv_node && do_null) {
		board.play_null<color>();

		int reduction = nmp_reduction(depth, beta, static_eval);
		int depth_prime = std::max(depth - reduction, 0);
		int null_eval = -pvs<~color>(board, depth_prime, ply + 1, -beta, -beta + 1, false);

		board.undo_null<color>();
		if (null_eval >= beta) return null_eval;
	}

	if (!in_check && !pv_node) {
		if (depth < RFP_MAX_DEPTH && static_eval >= beta + RFP_MARGIN * depth) {
			return static_eval;
		}
	}

	MoveList<color> all_legal_moves(board);
	if (all_legal_moves.size() == 0) {
		if (in_check) return -(MATE_SCORE - ply);
		return 0;
	}

	ScoredMoves scored_moves = order_moves<color>(all_legal_moves, board, ply);

	Move best_move = select_move(scored_moves, 0);
	int value = NEG_INF_CHESS;
	for (int move_idx = 0; move_idx < static_cast<int>(scored_moves.size()); move_idx++) {
		Move legal_move = select_move(scored_moves, move_idx);

		if (!pv_node && depth <= LMP_MIN_DEPTH && move_idx > depth * LMP_DEPTH_MULTIPLIER) {
			break;
		}

		board.play<color>(legal_move);
		data.nodes_searched += 1;
		int new_value = NEG_INF_CHESS;

		bool full_depth_zero_window;

		int reduction = lmr_reduction(pv_node, ply, in_check, move_idx, depth, legal_move);
		if (reduction > 0) {
			int new_depth = std::max(0, depth - reduction - 1);
			new_value = -pvs<~color>(board, new_depth, ply + 1, -alpha - 1, -alpha, true);
			full_depth_zero_window = new_value > alpha && new_depth != depth - 1;
		}
		else full_depth_zero_window = !pv_node || move_idx > 0;

		if (full_depth_zero_window) {
			new_value = -pvs<~color>(board, depth - 1, ply + 1, -alpha - 1, -alpha, true);
		}

		if (new_value == NEG_INF_CHESS || (pv_node && ((new_value > alpha && new_value < beta) || move_idx == 0))) {
			new_value = -pvs<~color>(board, depth - 1, ply + 1, -beta, -alpha, true);
		}


		board.undo<color>(legal_move);
		if (!data.search_completed) return 0;

		if (new_value > value) {
			value = new_value;

			update_pv(data.pv, ply, legal_move);
			if (ply == 0) data.value = value;
			best_move = legal_move;

			if (value > alpha) {
				alpha = value;
				if (alpha >= beta) {
					update_history<color>(scored_moves, best_move, depth, ply, move_idx);
					break;
				}
			}
		}
	}

	TranspositionTableEntryNodeType node_type = t_table.get_node_type(alpha_initial, beta, value);
	t_table.put(board.get_hash(), depth, value, ply, best_move, pv_node, node_type);
	return value;
}

template<Color color>
PVSData aspiration_windows(Position& board, int prev_score, short depth, int time_limit) {
	reset_data();
	data.time_limit = time_limit;
	int alpha = NEG_INF_CHESS;
	int beta = POS_INF_CHESS;
	int delta = ASP_WINDOW_INIT_DELTA;

	if (depth > ASP_WINDOW_MIN_DEPTH) {
		alpha = std::max(prev_score - ASP_WINDOW_INIT_WINDOW, NEG_INF_CHESS);
		beta  = std::min(prev_score + ASP_WINDOW_INIT_WINDOW, POS_INF_CHESS);
	}

	while (true) {
		if (alpha < -ASP_WINDOW_FULL_SEARCH_BOUNDS) alpha = NEG_INF_CHESS;
		if (beta  > ASP_WINDOW_FULL_SEARCH_BOUNDS) beta  = POS_INF_CHESS;

		pvs<color>(board, depth, 0, alpha, beta, false);
		int score = data.value;
		if (score <= alpha) {
			alpha = std::max(alpha - delta, NEG_INF_CHESS);
			beta = (alpha + 3 * beta) / 4;
		} else if (score >= beta) {
			beta = std::min(beta + delta, POS_INF_CHESS);
			depth -= 1;
		} else break;
		delta += delta * 2 / 3;
	}
	data.best_move = data.pv.table[0][0];
	return data;
}
