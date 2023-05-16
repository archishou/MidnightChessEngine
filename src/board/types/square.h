#pragma once

#include "../../types.h"
#include "board_types.h"

constexpr i32 NSQUARES = 64;

enum Square : i32 {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,
	NO_SQUARE
};

inline constexpr Square operator ++(Square& orig, i32) {
	Square r_val = orig;
	orig = static_cast<Square>(orig + 1);
	return r_val;
}

constexpr Square operator +(Square s, Direction d) {
	return Square(static_cast<i32>(s) + static_cast<i32>(d));
}

constexpr Square operator -(Square s, Direction d) {
	return Square(static_cast<i32>(s) - static_cast<i32>(d));
}

inline Square& operator +=(Square& s, Direction d) {
	return s = s + d;
}

inline Square& operator +=(Square& s, i32 i) {
	return s = static_cast<Square>(s + i);
}

inline Square& operator -=(Square& s, Direction d) {
	return s = s - d;
}

inline Square flip(Square s) {
	return static_cast<Square>(s ^ 0b111000);
}

constexpr Rank rank_of(Square s) {
	return Rank(s >> 3);
}

constexpr File file_of(Square s) {
	return File(s & 0b111);
}

constexpr i32 diagonal_of(Square s) {
	return 7 + rank_of(s) - file_of(s);
}

constexpr i32 anti_diagonal_of(Square s) {
	return rank_of(s) + file_of(s);
}

constexpr Square create_square(File f, Rank r) {
	return Square(r << 3 | f);
}

inline const string SQ_TO_STRING[NSQUARES + 1] = {
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
		"None"
};
