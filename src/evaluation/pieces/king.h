#pragma once

#include "evaluation/types.h"
#include "evaluation/constants/misc.h"
#include "evaluation/bitboards.h"
#include "evaluation/constants/psts.h"

constexpr int NKING_SIDES = 2;

constexpr Bitboard KING_SAFE_AREA[NCOLORS] = {
		0xC3D7, 0xd7c3000000000000
};

constexpr Bitboard PAWN_SHIELD[NKING_SIDES][NCOLORS] = {
		{0x700, 0x7000000000000},
		{0xe000, 0xe0000000000000}
};

template<Color Us>
Score evaluate_king(const Position& board);