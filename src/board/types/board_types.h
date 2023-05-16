#pragma once
#include "../../types.h"

using ZobristHash = u64;
using Bitboard = u64;

constexpr i32 NCOLORS = 2;
enum Color : i32 {
	WHITE,
	BLACK,
};

constexpr Color operator~(Color c) {
	return Color(c ^ BLACK);
}

constexpr i32 NDIRS = 8;
enum Direction : i32 {
	NORTH = 8, NORTH_EAST = 9, EAST = 1, SOUTH_EAST = -7,
	SOUTH = -8, SOUTH_WEST = -9, WEST = -1, NORTH_WEST = 7,
	NORTH_NORTH = 16, SOUTH_SOUTH = -16
};

template<Color C, Direction D>
consteval Direction relative_dir() {
	if constexpr (C == WHITE) return D;
	return Direction(-D);
}

constexpr i32 NFILES = 8;
using File = i32;

constexpr File AFILE = 0;
constexpr File BFILE = 1;
constexpr File CFILE = 2;
constexpr File DFILE = 3;
constexpr File EFILE = 4;
constexpr File FFILE = 5;
constexpr File GFILE = 6;
constexpr File HFILE = 7;

constexpr i32 NRANKS = 8;
using Rank = i32;

constexpr Rank RANK1 = 0;
constexpr Rank RANK2 = 1;
constexpr Rank RANK3 = 2;
constexpr Rank RANK4 = 3;
constexpr Rank RANK5 = 4;
constexpr Rank RANK6 = 5;
constexpr Rank RANK7 = 6;
constexpr Rank RANK8 = 7;

template<Color color>
constexpr Rank relative_rank(Rank r) {
	if constexpr (color == WHITE) return r;
	return RANK8 - r;
}

constexpr i32 NCASTLING_RIGHTS = 4;
using CastleRight = i32;

constexpr CastleRight BLACK_OOO = 0b0001;
constexpr CastleRight BLACK_OO	= 0b0010;
constexpr CastleRight WHITE_OOO = 0b0100;
constexpr CastleRight WHITE_OO	= 0b1000;
