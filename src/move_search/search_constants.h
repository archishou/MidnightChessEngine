#pragma once
#include "../types.h"

constexpr i32 POS_INF_CHESS = 1000000;
constexpr i32 NEG_INF_CHESS = -POS_INF_CHESS;
constexpr i32 MATE_SCORE = POS_INF_CHESS / 10;
constexpr i16 MAX_DEPTH = 100;
constexpr i16 MAX_PLY = MAX_DEPTH;
constexpr i32 MATE_BOUND = MATE_SCORE - MAX_DEPTH;
constexpr i32 DEFAULT_SEARCH_TIME = 86'400'000;
