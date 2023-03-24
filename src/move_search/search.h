//
// Created by Archishmaan on 12/21/22.
//
#pragma once
#include <fstream>
#include <vector>
#include <random>
#include "move_generation/position.h"
#include "pvs.h"
#include "search_constants.h"
#include "utils/fen_constants.h"

struct BestMoveSearchParameters {
	short depth = MAX_DEPTH;
	int hard_time_limit = DEFAULT_SEARCH_TIME;
	int soft_time_limit = DEFAULT_SEARCH_TIME;
	bool debug_info = false;
};

const struct BestMoveSearchParameters DEFAULT_BEST_MOVE_SEARCH_PARAMS = {
		.depth = MAX_DEPTH,
		.hard_time_limit = DEFAULT_SEARCH_TIME,
		.soft_time_limit = DEFAULT_SEARCH_TIME,
		.debug_info = true
};

typedef Move Line[MAX_DEPTH];
const int LINE_SIZE = MAX_DEPTH;

struct BestMoveSearchResults {
	Move best_move = Move();
	Line pv;
	int depth_searched = 0;
	int seldepth = 0;

	int time_searched = 0;
	int value = 0;

	uint64_t q_nodes_searched = 0;
	uint64_t nodes_searched = 0;
	double nodes_per_second = 0;
};

std::ostream& operator<<(std::ostream& os, const Line& line) {
	for(const Move & i : line) {
		if (i == 0) break;
		os << i << " ";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const BestMoveSearchResults& results) {
	os << "Best Move: " << results.best_move << std::endl;
	os << "Principal Variation: " << results.pv << std::endl;
	os << "Depth Searched: " << results.depth_searched << std::endl;
	os << "Seldepth: " << results.seldepth << std::endl;
	os << "Evaluation: " << results.value << std::endl;
	os << "Time Searched: " << results.time_searched << std::endl;
	os << "Nodes Searched: " << results.nodes_searched << std::endl;
	os << "Node / Second: " << results.nodes_per_second << std::endl;
	os << "Q Nodes Searched: " << results.q_nodes_searched << std::endl;
	return os;
}

void update_best_move_results(BestMoveSearchResults& search_results, PVSData& ab_results, int sub_depth, bool debug) {
	search_results.value = ab_results.value;
	search_results.best_move = ab_results.best_move;
	search_results.depth_searched = sub_depth;
	search_results.nodes_searched += ab_results.nodes_searched + ab_results.q_nodes_searched;
	search_results.seldepth = ab_results.seldepth;
	search_results.time_searched = get_elapsed_time(Milliseconds);
	for (int i = 0; i < ab_results.pv.length[0]; i++) {
		search_results.pv[i] = ab_results.pv.table[0][i];
	}
	if (debug) {
		std::cout << "info depth " << sub_depth << " seldepth " << search_results.seldepth <<  " score cp " << search_results.value << " time " <<
		search_results.time_searched << " nodes " << search_results.nodes_searched << " pv " << search_results.pv << std::endl;
	}
}

template<Color color>
BestMoveSearchResults iterative_deepening(Position& board, const BestMoveSearchParameters& params) {
	struct BestMoveSearchResults search_results;

	reset_clock();
	for (int sub_depth = 1; sub_depth <= params.depth; sub_depth++) {
		if (time_elapsed_exceeds(params.soft_time_limit, Milliseconds)) {
			break;
		}
		struct PVSData ab_results = pvs_root<color>(board, sub_depth, params.hard_time_limit);
		if (ab_results.search_completed) {
			std::memset(search_results.pv, 0, sizeof(search_results.pv));
			update_best_move_results(search_results, ab_results, sub_depth, params.debug_info);
		}
	}

	search_results.time_searched = get_elapsed_time(Seconds);
	search_results.nodes_per_second = search_results.nodes_searched / (search_results.time_searched + 1);

	return search_results;
}

template<Color color>
BestMoveSearchResults best_move(Position& board, const BestMoveSearchParameters& parameters) {
	return iterative_deepening<color>(board, parameters);
}

template<Color color>
BestMoveSearchResults best_move(Position& board) {
	return best_move<color>(board, DEFAULT_BEST_MOVE_SEARCH_PARAMS);
}

BestMoveSearchResults best_move(Position& board, const BestMoveSearchParameters& parameters) {
	if (board.turn() == BLACK) return best_move<BLACK>(board, parameters);
	return best_move<WHITE>(board, parameters);
}

BestMoveSearchResults best_move(Position& board) {
	return best_move(board, DEFAULT_BEST_MOVE_SEARCH_PARAMS);
}

