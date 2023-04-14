//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once

#include <bit>
#include <cstdint>

typedef int Score;
constexpr Score SCORE_ZERO = 0;

constexpr Score S(int mg, int eg) {
	return Score((int)((unsigned int)eg << 16) + mg);
}

inline int eg_value(Score s) {
	const uint16_t eg = static_cast<uint16_t>(static_cast<uint32_t>(s + 0x8000) >> 16);
	return static_cast<int>(std::bit_cast<int16_t>(eg));
}

inline int mg_value(Score s) {
	const uint16_t mg = static_cast<uint16_t>(s);
	return static_cast<int>(std::bit_cast<int16_t>(mg));
}
