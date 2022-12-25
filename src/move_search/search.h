//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "position.h"
#include "src/evaluation/evaluate.h"

const int POS_INF_CHESS = 100000;
const int NEG_INF_CHESS = -POS_INF_CHESS;

template<Color color>
int alphaBeta(Position &board, int depth, int alpha, int beta) {
	if (depth == 0) return evaluate<color>(board);
	MoveList<color> allLegalMoves(board);
	int value = INT_MIN;
	for (Move legalMove : allLegalMoves)  {
		board.play<color>(legalMove);
		int v = -alphaBeta<~color>(board, depth - 1, -beta, -alpha);
		board.undo<color>(legalMove);
		value = std::max(value, v);
		alpha = std::max(alpha, value);
		if (alpha >= beta) break;
	}
	return value;
}

template<Color color>
Move alphaBetaRoot(Position &board, int depth) {
	int alpha = NEG_INF_CHESS, beta = POS_INF_CHESS, maxValue = INT_MIN;
	MoveList<color> allLegalMoves(board);
	Move bestEvaluatedMove = *allLegalMoves.begin();
	for (Move legalMove : allLegalMoves)  {
		board.play<color>(legalMove);
		int value = -alphaBeta<~color>(board, depth - 1, -beta, -alpha);
		board.undo<color>(legalMove);
		if (value > maxValue) {
			bestEvaluatedMove = legalMove;
			maxValue = value;
		}
	}
	return bestEvaluatedMove;
}

template<Color color>
Move bestMove(Position& board) {
	return alphaBetaRoot<color>(board, 6);
}
