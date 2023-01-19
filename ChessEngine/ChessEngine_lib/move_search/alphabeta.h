//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "move_generation/position.h"
#include "move_ordering.h"
#include "transposition_table.h"

struct AlphaBetaData {
    Move best_move;
    bool search_completed;
	int value;
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
int q_search(Position& board, int alpha, int beta, AlphaBetaData& data, TimePoint end_time) {
	data.search_completed = true;
	if (position_is_draw(board)) {
		return 0;
	}
	int value = NEG_INF_CHESS;

	int v = evaluate<color>(board);
	value = std::max(value, v);
	alpha = std::max(alpha, value);
	if (alpha >= beta) {
		return v;
	}

	MoveList<color> capture_moves(board, QSearchMoveGenerationsOptions);
	ScoredMoves scored_moves = order_moves<color>(capture_moves, board);
	if (scored_moves.empty()) {
		return v;
	}
	for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
		if (exceededTime(end_time)) {
			data.search_completed = false;
			return value;
		}
		board.play<color>(legal_move);
		v = -q_search<~color>(board, -beta, -alpha, data, end_time);
		board.undo<color>(legal_move);
		value = std::max(value, v);
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}
	return value;
}

template<Color color>
int alpha_beta(Position& board, int depth, int ply, int alpha, int beta, AlphaBetaData& data, TimePoint end_time) {
	data.search_completed = true;
	if (ply > 0) {
		if (position_is_draw(board)) { return 0; }
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) {
			return alpha;
		}
	}
	if (depth == 0) {
		return q_search<color>(board, alpha, beta, data, end_time);
	}
	MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board);

	if (scored_moves.empty()) {
		if (board.in_check<color>()) return -(MATE_SCORE - ply);
		return 0;
	}

	Move best_move = scored_moves.begin()->move;
	int value = NEG_INF_CHESS;
	// No legal moves, return -inf!
	for (const ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
		if (exceededTime(end_time)) {
			data.search_completed = false;
			return value;
		}
		board.play<color>(legal_move);
		const int v = -alpha_beta<~color>(board, depth - 1, ply + 1, -beta, -alpha, data, end_time);
		board.undo<color>(legal_move);
		if (v > value) best_move = legal_move;
		value = std::max(value, v);
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}
	if (ply == 0) data.best_move = best_move;
	return value;
}

template<Color color>
AlphaBetaData alpha_beta_root(Position& board, int depth, TimePoint end_time) {
	AlphaBetaData data;
	alpha_beta<color>(board, depth, 0, NEG_INF_CHESS, POS_INF_CHESS, data, end_time);
	return data;
}