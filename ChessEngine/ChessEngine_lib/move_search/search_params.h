//
// Created by Archishmaan Peyyety on 1/1/23.
//

const int POS_INF_CHESS = 1000000;
const int NEG_INF_CHESS = -POS_INF_CHESS;
const int MATE_SCORE = POS_INF_CHESS / 10;
const short MAX_DEPTH = 100;
const int MAX_MATE_DEPTH = 10 * MAX_DEPTH;

struct BestMoveSearchParameters {
	short depth;
	int time_limit;
};

const struct BestMoveSearchParameters DEFAULT_BEST_MOVE_SEARCH_PARAMS = {
	.depth = MAX_DEPTH,
	.time_limit = 1000
};
