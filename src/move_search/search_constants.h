#pragma once
#include "../types.h"

constexpr auto POS_INF_CHESS = 1000000;
constexpr auto NEG_INF_CHESS = -POS_INF_CHESS;
constexpr auto MATE_SCORE = POS_INF_CHESS / 10;
constexpr i16 MAX_DEPTH = 100;
constexpr i16 MAX_PLY = MAX_DEPTH;
constexpr auto MATE_BOUND = MATE_SCORE - MAX_DEPTH;
constexpr auto DEFAULT_SEARCH_TIME = 86'400'000;
