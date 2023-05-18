//
// Created by Archishmaan Peyyety on 2/24/23.
//
#pragma once
#include "evaluation/bitboards.h"
#include "evaluation/types.h"
#include "evaluation/constants/psts.h"
#include "evaluation/constants/misc.h"

template<Color c>
inline Score evaluate_doubled_pawns(const Position& board) {
	return DOUBLED_PAWN_PENALTY * pop_count(doubled_pawns<c>(board));
}

template<Color c>
inline Score evaluate_isolated_pawns(const Position& board) {
	return ISOLATED_PAWN_PENALTY * pop_count(isolated_pawns<c>(board));
}

template<Color c>
Score evaluate_passed_pawns(const Position& board);

template<Color c>
Score evaluate_pawn_phalanx(const Position& board);

template<Color c>
Score evaluate_pawn_locations(const Position& board);

template<Color c>
Score evaluate_pawn_structure(const Position& board);
