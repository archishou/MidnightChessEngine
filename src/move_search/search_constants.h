#pragma once
constexpr int POS_INF_CHESS = 1000000;
constexpr int NEG_INF_CHESS = -POS_INF_CHESS;
constexpr int MATE_SCORE = POS_INF_CHESS / 10;
constexpr short MAX_DEPTH = 100;
constexpr short MAX_PLY = MAX_DEPTH;
constexpr int MATE_BOUND = MATE_SCORE - MAX_DEPTH;
constexpr int DEFAULT_SEARCH_TIME = 1000;
