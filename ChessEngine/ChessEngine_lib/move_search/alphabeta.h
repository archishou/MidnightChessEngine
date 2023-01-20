//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "move_generation/position.h"
#include "move_ordering.h"
#include "pv_table.h"

struct AlphaBetaData {
    Move best_move;
    bool search_completed;
	int value;
	// triangular-table-table
	PV pv;
};

struct MoveGenerationOptions QSearchMoveGenerationsOptions = {
	.generate_captures = true,
	.generate_checks = false,
	.generate_promotion = false,
	.generate_quiet = false,
};

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

bool exceededTime(TimePoint endTime) {
	return std::chrono::system_clock::now() > endTime;
}

bool position_is_draw(Position &board) {
	uint64_t current_hash = board.get_hash();
	int count = 0;
	for (uint64_t hash : board.hash_history) {
		if (hash == current_hash) count += 1;
		if (count >= 2) return true;
	}
	return false;
}

template<Color color>
int q_search(Position& board, int alpha, int beta, AlphaBetaData& data, TimePoint end_time, TranspositionTable& t_table) {
	data.search_completed = true;
	if (position_is_draw(board)) return 0;
	int value = NEG_INF_CHESS;

	int v = evaluate<color>(board);
	value = std::max(value, v);
	alpha = std::max(alpha, value);
	if (alpha >= beta) return v;

	MoveList<color> capture_moves(board, QSearchMoveGenerationsOptions);
	ScoredMoves scored_moves = order_moves<color>(capture_moves, board, t_table);
	if (scored_moves.empty()) return v;
	for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
		if (exceededTime(end_time)) {
			data.search_completed = false;
			return value;
		}
		board.play<color>(legal_move);
		v = -q_search<~color>(board, -beta, -alpha, data, end_time, t_table);
		board.undo<color>(legal_move);
		value = std::max(value, v);
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}
	return value;
}

template<Color color>
int alpha_beta(Position& board, int depth, int ply, int alpha, int beta, AlphaBetaData& data, TimePoint end_time, TranspositionTable& t_table) {
	data.search_completed = true;
	int alpha_initial = alpha;
	init_pv(data.pv, ply);
	if (ply > 0) {
		if (position_is_draw(board)) { return 0; }
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) {
			return alpha;
		}
	}

	TranspositionTableSearchResults probe_results = t_table.probe(board.get_hash(), depth);
	if (probe_results.entry_found) {
		TranspositionTableEntry tt_entry = probe_results.entry;
		if (tt_entry.node_type == EXACT) {
			return tt_entry.value;
		} else if (tt_entry.node_type == LOWER_NODE) {
			alpha = std::max(alpha, tt_entry.value);
		} else if (tt_entry.node_type == UPPER_NODE) {
			beta = std::max(beta, tt_entry.value);
		}
		if (alpha >= beta) return tt_entry.value;
	}

	if (depth == 0) {
		return q_search<color>(board, alpha, beta, data, end_time, t_table);
	}
	MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board, t_table);

	if (scored_moves.empty()) {
		if (board.in_check<color>()) return -(MATE_SCORE - ply);
		return 0;
	}

	Move best_move = scored_moves.begin()->move;
	int value = NEG_INF_CHESS;
	for (const ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
		if (exceededTime(end_time)) {
			data.search_completed = false;
			return value;
		}
		board.play<color>(legal_move);
		const int v = -alpha_beta<~color>(board, depth - 1, ply + 1, -beta, -alpha, data, end_time, t_table);
		board.undo<color>(legal_move);
		if (v > value) best_move = legal_move;
		if (v > alpha) update_pv(data.pv, ply, best_move);
		value = std::max(value, v);
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}
	TranspositionTableEntryNodeType node_type = t_table.get_node_type(alpha_initial, beta, value);
	t_table.put(board.get_hash(), depth, value, best_move, node_type);
	return value;
}

template<Color color>
AlphaBetaData alpha_beta_root(Position& board, int depth, TimePoint end_time, TranspositionTable& t_table) {
	AlphaBetaData data;
	alpha_beta<color>(board, depth, 0, NEG_INF_CHESS, POS_INF_CHESS, data, end_time, t_table);
	data.best_move = data.pv.table[0][0];
	return data;
}