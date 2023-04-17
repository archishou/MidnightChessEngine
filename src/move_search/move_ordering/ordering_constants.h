//
// Created by Archishmaan Peyyety on 2/14/23.
//
#pragma once
#include "evaluation/constants/misc.h"
const int PREVIOUS_BEST_MOVE_BONUS = 10'000'000;
const int PROMOTION_BONUS = PREVIOUS_BEST_MOVE_BONUS / 10;  // 1'000'000
const int MVV_LVA_BONUS = PROMOTION_BONUS / 10;  // 100'000
const int KILLER_MOVE_BONUS = MVV_LVA_BONUS / 10;  // 10'000
const int IN_OPP_PAWN_TERRITORY_PENALTY = -350;

const short NKILLERS = 2;

constexpr int ORDERING_PAWN_VALUE = 100;
constexpr int ORDERING_KNIGHT_VALUE = 320;
constexpr int ORDERING_BISHOP_VALUE = 340;
constexpr int ORDERING_ROOK_VALUE = 500;
constexpr int ORDERING_QUEEN_VALUE = 900;
constexpr int ORDERING_KING_VALUE = 1000;
constexpr int ORDERING_PIECE_VALUES[NPIECE_TYPES] = {
	ORDERING_PAWN_VALUE, ORDERING_KNIGHT_VALUE, ORDERING_BISHOP_VALUE, ORDERING_ROOK_VALUE, ORDERING_QUEEN_VALUE, 0
};
const int SEE_VALUES[NPIECE_TYPES] = {
	(mg_value(PIECE_VALUES[PAWN]) + eg_value(PIECE_VALUES[PAWN])) / 2,
	(mg_value(PIECE_VALUES[KNIGHT]) + eg_value(PIECE_VALUES[KNIGHT])) / 2,
	(mg_value(PIECE_VALUES[BISHOP]) + eg_value(PIECE_VALUES[BISHOP])) / 2,
	(mg_value(PIECE_VALUES[ROOK]) + eg_value(PIECE_VALUES[ROOK])) / 2,
	(mg_value(PIECE_VALUES[QUEEN]) + eg_value(PIECE_VALUES[QUEEN])) / 2,
	0
};