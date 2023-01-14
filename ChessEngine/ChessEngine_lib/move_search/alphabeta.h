//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "move_generation/position.h"
#include "move_ordering.h"

struct AlphaBetaResults {
    Move best_move;
    bool search_completed;
	int value;
	int nodes_searched;
};

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

bool exceededTime(AlphaBetaResults& results, TimePoint endTime) {
	if (results.nodes_searched % 1024 == 0) {
		return std::chrono::system_clock::now() > endTime;
	}
	return false;
}

bool position_is_draw(Position &board) {
	uint64_t current_hash = board.get_hash();
	int count = 0;
	for (uint64_t hash : board.hash_history) {
		if (hash == current_hash) count += 1;
		if (count >= 3) return true;
	}
	return false;
}

template<Color color>
int q_search(Position& board, int alpha, int beta, AlphaBetaResults& ab_results, TimePoint end_time) {
	if (position_is_draw(board)) return 0;
	MoveList<color> all_legal_moves(board);
	std::vector<Move> capture_moves = filter_moves_only_captures(board, all_legal_moves);
	ScoredMoves scored_moves = order_moves<color>(capture_moves, board);
	if (scored_moves.empty()) { return evaluate<color>(board); }
	int value = NEG_INF_CHESS;
	for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
		if (exceededTime(ab_results, end_time)) return value;
		board.play<color>(legal_move);
		int v = -q_search<~color>(board, -beta, -alpha, end_time);
		board.undo<color>(legal_move);
		value = std::max(value, v);
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}
	return value;
}

template<Color color>
int alpha_beta(Position &board, int depth, int alpha, int beta, AlphaBetaResults& ab_results, TimePoint end_time) {
	if (position_is_draw(board)) return 0;
    if (depth == 0) {
		return evaluate<color>(board);
	}
    MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board);
    int value = NEG_INF_CHESS;
    // Auto handles checkmate, no legal moves, return -inf!
    for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
        if (exceededTime(ab_results, end_time)) return value;
        board.play<color>(legal_move);
        int v = -alpha_beta<~color>(board, depth - 1, -beta, -alpha, ab_results, end_time);
        board.undo<color>(legal_move);
		ab_results.nodes_searched += 1;
        value = std::max(value, v);
        alpha = std::max(alpha, value);
        if (alpha >= beta) break;
    }
    return value;
}

template<Color color>
AlphaBetaResults alpha_beta_root(Position &board, int depth, TimePoint end_time) {
    struct AlphaBetaResults ab_results;
    int alpha = NEG_INF_CHESS, beta = POS_INF_CHESS, max_value = NEG_INF_CHESS;
    MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board);

	ab_results.search_completed = false;
	ab_results.best_move = scored_moves.begin()->move;

    for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
		if (exceededTime(ab_results, end_time)) return ab_results;
        board.play<color>(legal_move);
        int value = -alpha_beta<~color>(board, depth - 1, -beta, -alpha, ab_results, end_time);
        board.undo<color>(legal_move);
        if (value > max_value) {
            ab_results.best_move = legal_move;
			max_value = value;
        }
    }
	ab_results.search_completed = true;
	ab_results.value = max_value;
    return ab_results;
}
