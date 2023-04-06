//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once

typedef int Score;
constexpr Score SCORE_ZERO = 0;

constexpr Score S(int mg, int eg) {
	return Score((int)((unsigned int)eg << 16) + mg);
}

inline int eg_value(Score s) {
	union { uint16_t u; int16_t s; } eg = { uint16_t(unsigned(s + 0x8000) >> 16) };
	return int(eg.s);
}

inline int mg_value(Score s) {
	union { uint16_t u; int16_t s; } mg = { uint16_t(unsigned(s)) };
	return int(mg.s);
}
