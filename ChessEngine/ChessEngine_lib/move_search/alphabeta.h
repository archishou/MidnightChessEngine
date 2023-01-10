//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "move_generation/position.h"
#include "move_ordering.h"

struct AlphaBetaResults {
    Move best_move;
    bool search_completed;
};

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

bool exceededTime(TimePoint endTime) {
    return std::chrono::system_clock::now() > endTime;
}

template<Color color>
int alpha_beta(Position &board, int depth, int alpha, int beta, TimePoint endTime) {
    if (depth == 0) return evaluate<color>(board);
    MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board);
    int value = NEG_INF_CHESS;
    // Auto handles checkmate, no legal moves, return -inf!
    for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
        if (exceededTime(endTime)) return value;
        board.play<color>(legal_move);
        int v = -alpha_beta<~color>(board, depth - 1, -beta, -alpha, endTime);
        board.undo<color>(legal_move);
        value = std::max(value, v);
        alpha = std::max(alpha, value);
        if (alpha >= beta) break;
    }
    return value;
}

template<Color color>
AlphaBetaResults alpha_beta_root(Position &board, int depth, TimePoint end_time) {
    struct AlphaBetaResults results;
    int alpha = NEG_INF_CHESS, beta = POS_INF_CHESS, max_value = NEG_INF_CHESS;
    MoveList<color> all_legal_moves(board);
	ScoredMoves scored_moves = order_moves(all_legal_moves, board);

    results.search_completed = false;
    results.best_move = scored_moves.begin()->move;

    for (ScoredMove scored_move : scored_moves) {
		Move legal_move = scored_move.move;
        if (exceededTime(end_time)) return results;
        board.play<color>(legal_move);
        int value = -alpha_beta<~color>(board, depth - 1, -beta, -alpha, end_time);
        board.undo<color>(legal_move);
        if (value > max_value) {
            results.best_move = legal_move;
			max_value = value;
        }
    }
    results.search_completed = true;
    return results;
}
