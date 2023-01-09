//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include "search_params.h"
#include "position.h"
#include "evaluate.h"

struct AlphaBetaResults {
    Move bestMove;
    bool searchCompleted;
};

typename std::chrono::system_clock sys_clock;

template<typename Clock>
bool exceededTime(std::chrono::time_point<Clock> endTime) {
    return std::chrono::system_clock::now() > endTime;
}

template<Color color, typename Clock>
int alphaBeta(Position &board, int depth, int alpha, int beta, std::chrono::time_point<Clock> endTime) {
    if (depth == 0) return evaluate<color>(board);
    MoveList<color> allLegalMoves(board);
    int value = NEG_INF_CHESS;
    // Auto handles checkmate, no legal moves, return -inf!
    for (Move legalMove : allLegalMoves)  {
        if (exceededTime(endTime)) return value;
        board.play<color>(legalMove);
        int v = -alphaBeta<~color, std::chrono::system_clock>(board, depth - 1, -beta, -alpha, endTime);
        board.undo<color>(legalMove);
        value = std::max(value, v);
        alpha = std::max(alpha, value);
        if (alpha >= beta) break;
    }
    return value;
}

template<Color color, typename Clock>
AlphaBetaResults alpha_beta_root(Position &board, int depth, std::chrono::time_point<Clock> end_time) {
    struct AlphaBetaResults results;
    int alpha = NEG_INF_CHESS, beta = POS_INF_CHESS, maxValue = NEG_INF_CHESS;
    MoveList<color> allLegalMoves(board);

    results.searchCompleted = false;
    results.bestMove = *allLegalMoves.begin();

    for (Move legalMove : allLegalMoves)  {
        if (exceededTime(end_time)) return results;
        board.play<color>(legalMove);
        int value = -alphaBeta<~color, std::chrono::system_clock>(board, depth - 1, -beta, -alpha, end_time);
        board.undo<color>(legalMove);
        if (value > maxValue) {
            results.bestMove = legalMove;
            maxValue = value;
        }
    }
    results.searchCompleted = true;
    return results;
}
