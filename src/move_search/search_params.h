//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once
const int POS_INF_CHESS = 1000000;
const int NEG_INF_CHESS = -POS_INF_CHESS;
const int MATE_SCORE = POS_INF_CHESS / 10;
const short MAX_DEPTH = 100;
const short MAX_PLY = MAX_DEPTH;
const int MATE_BOUND = MATE_SCORE - MAX_DEPTH;
const int DEFAULT_SEARCH_TIME = 1000;

const int RFP_MARGIN = 75;
const int RFP_MAX_DEPTH = 9;

const double LMR_BASE = 0.75;
const double LMR_DIVISOR = 2.25;

struct BestMoveSearchParameters {
	short depth = MAX_DEPTH;
	int time_limit = DEFAULT_SEARCH_TIME;
	bool debug_info = false;
};

const struct BestMoveSearchParameters DEFAULT_BEST_MOVE_SEARCH_PARAMS = {
	.depth = MAX_DEPTH,
	.time_limit = DEFAULT_SEARCH_TIME,
	.debug_info = true
};
