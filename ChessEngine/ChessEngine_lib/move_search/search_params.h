//
// Created by Archishmaan Peyyety on 1/1/23.
//

const int POS_INF_CHESS = 1000000;
const int NEG_INF_CHESS = -POS_INF_CHESS;
const int MATE_SCORE = POS_INF_CHESS / 10;
const short MAX_DEPTH = 100;
const int MATE_BOUND = MATE_SCORE - MAX_DEPTH;
const int DEFAULT_SEARCH_TIME = 1000;

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
