//
// Created by Archishmaan on 12/21/22.
//

#include "position.h"

const int PAWN_VALUE = 1;
const int KNIGHT_VALUE = 3;
const int BISHOP_VALUE = 3;
const int ROOK_VALUE = 5;
const int QUEEN_VALUE = 8;
const int CHECK_PENALTY = -1;

template<Color color>
int evaluateOneSide(Position& board) {
	// TODO: better ways to handle mate, this is not ideal and can result in mis-evaluations of a position.
	int usPawns = pop_count(board.bitboard_of(color, PAWN));
	int usKnight = pop_count(board.bitboard_of(color, KNIGHT));
	int usBishop = pop_count(board.bitboard_of(color, BISHOP));
	int usRook = pop_count(board.bitboard_of(color, ROOK));
	int usQueen = pop_count(board.bitboard_of(color, QUEEN));
	int usCheck = board.in_check<color>() ? CHECK_PENALTY: 0;

	return (PAWN_VALUE * usPawns) + (KNIGHT_VALUE * usKnight) + (BISHOP_VALUE * usBishop) +
		(ROOK_VALUE * usRook) + (QUEEN_VALUE * usQueen) + usCheck;
}

template<Color color>
int evaluate(Position& board) {
	return evaluateOneSide<color>(board) - evaluateOneSide<~color>(board);
}
