#pragma once
const int POS_INF_CHESS = 1000000;
const int NEG_INF_CHESS = -POS_INF_CHESS;
const int MATE_SCORE = POS_INF_CHESS / 10;
const short MAX_DEPTH = 100;
const short MAX_PLY = MAX_DEPTH;
const int MATE_BOUND = MATE_SCORE - MAX_DEPTH;
const int DEFAULT_SEARCH_TIME = 1000;

