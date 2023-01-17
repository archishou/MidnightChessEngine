//
// Created by Archishmaan Peyyety on 1/1/23.
//

const int POS_INF_CHESS = 1000000;
const int NEG_INF_CHESS = -POS_INF_CHESS;

struct BestMoveSearchParameters {
	int depth;
	int time_limit;
};

const struct BestMoveSearchParameters DEFAULT_BEST_MOVE_SEARCH_PARAMS = {
	.depth = 100,
	.time_limit = 1000
};
