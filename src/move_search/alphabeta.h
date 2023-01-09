//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "position.h"
#include "move_ordering.h"

struct AlphaBetaResults {
    Move best_move;
    bool search_completed;
};

typename std::chrono::system_clock sys_clock;

template<typename Clock>
bool exceededTime(std::chrono::time_point<Clock> endTime) {
    return std::chrono::system_clock::now() > endTime;
}

template<Color color, typename Clock>
int alpha_beta(Position &board, int depth, int alpha, int beta, std::chrono::time_point<Clock> endTime) {
    if (depth == 0) return evaluate<color>(board);
    MoveList<color> all_legal_moves(board);
    int value = NEG_INF_CHESS;
    // Auto handles checkmate, no legal moves, return -inf!
    for (Move legal_move : all_legal_moves)  {
        if (exceededTime(endTime)) return value;
        board.play<color>(legal_move);
        int v = -alpha_beta<~color, std::chrono::system_clock>(board, depth - 1, -beta, -alpha, endTime);
        board.undo<color>(legal_move);
        value = std::max(value, v);
        alpha = std::max(alpha, value);
        if (alpha >= beta) break;
    }
    return value;
}

template<Color color, typename Clock>
AlphaBetaResults alpha_beta_root(Position &board, int depth, std::chrono::time_point<Clock> end_time) {
    struct AlphaBetaResults results;
    int alpha = NEG_INF_CHESS, beta = POS_INF_CHESS, max_value = NEG_INF_CHESS;
    MoveList<color> all_legal_moves(board);

    results.search_completed = false;
    results.best_move = *all_legal_moves.begin();

    for (Move legal_move : all_legal_moves)  {
        if (exceededTime(end_time)) return results;
        board.play<color>(legal_move);
        int value = -alpha_beta<~color, std::chrono::system_clock>(board, depth - 1, -beta, -alpha, end_time);
        board.undo<color>(legal_move);
        if (value > max_value) {
            results.best_move = legal_move;
			max_value = value;
        }
    }
    results.search_completed = true;
    return results;
}
