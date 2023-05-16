#pragma once

#include "../board/types/bitboard.h"
#include "../board/types/board_types.h"

constexpr Bitboard WHITE_OO_BLOCKERS_MASK	= 0x60;
constexpr Bitboard WHITE_OOO_DANGER_MASK	= 0xC;
constexpr Bitboard WHITE_OOO_BLOCKERS_MASK	= 0xE;

constexpr Bitboard BLACK_OO_BLOCKERS_MASK	= 0x6000000000000000;
constexpr Bitboard BLACK_OOO_DANGER_MASK	= 0xC00000000000000;
constexpr Bitboard BLACK_OOO_BLOCKERS_MASK	= 0xE00000000000000;

template<Color C>
Bitboard oo_blockers_mask() {
	if constexpr (C == WHITE) return WHITE_OO_BLOCKERS_MASK;
	return BLACK_OO_BLOCKERS_MASK;
}

template<Color C>
Bitboard ooo_danger_mask() {
	if constexpr (C == WHITE) return WHITE_OOO_DANGER_MASK;
	return BLACK_OOO_DANGER_MASK;
}

template<Color C>
Bitboard ooo_blockers_mask() {
	if constexpr (C == WHITE) return WHITE_OOO_BLOCKERS_MASK;
	return BLACK_OOO_BLOCKERS_MASK;
}