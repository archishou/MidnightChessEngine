//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once

#include <cstring>
#include <cstdint>

typedef int Score;
constexpr Score SCORE_ZERO = 0;

constexpr Score S(int mg, int eg) {
	return Score(static_cast<int>(static_cast<unsigned int>(eg) << 16) + mg);
}

inline int eg_value(Score s) {
	const uint16_t eg = static_cast<uint16_t>(static_cast<uint32_t>(s + 0x8000) >> 16);

	int16_t v;
	std::memcpy(&v, &eg, sizeof(eg));

	return static_cast<int>(v);
}

inline int mg_value(Score s) {
	const uint16_t mg = static_cast<uint16_t>(s);

	int16_t v;
	std::memcpy(&v, &mg, sizeof(mg));

	return static_cast<int>(v);
}
