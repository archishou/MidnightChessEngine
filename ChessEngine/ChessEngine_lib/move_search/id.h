//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#include "move_generation/position.h"
#include "alphabeta.h"

typedef Move Line[MAX_DEPTH];
const int LINE_SIZE = MAX_DEPTH;

std::ostream& operator<<(std::ostream& os, const Line& line) {
	for(const Move & i : line) {
		if (i == 0) break;
		os << i << " ";
	}
	return os;
}

struct IDResults {
    Move best_move;
	Line pv;
    int depth_searched;
    double time_searched;
	int value;

	int nodes_searched;
	double nodes_per_second;
};

void update_id_results(IDResults& id_results, AlphaBetaData& ab_results, int sub_depth) {
	id_results.value = ab_results.value;
	id_results.best_move = ab_results.best_move;
	id_results.depth_searched = sub_depth;
	id_results.nodes_searched = ab_results.nodes_searched;
	for (int i = 0; i < ab_results.pv.length[0]; i++) {
		id_results.pv[i] = ab_results.pv.table[0][i];
	}
}

template<Color color>
IDResults iterative_deepening(Position& board, int time_limit, int depth) {
    struct IDResults id_results;
    std::chrono::time_point target_end_time = std::chrono::system_clock::now() +
			std::chrono::milliseconds(time_limit);

    std::clock_t start;
    start = std::clock();

	TranspositionTable t_table = TranspositionTable();

    for (int sub_depth = 1; sub_depth <= depth; sub_depth++) {
        struct AlphaBetaData ab_results =
				alpha_beta_root<color>(board, sub_depth, target_end_time, t_table);
        if (ab_results.search_completed) {
			update_id_results(id_results, ab_results, sub_depth);
        }
    }

	id_results.time_searched = (std::clock() - start ) / (double) CLOCKS_PER_SEC;
	id_results.nodes_per_second = id_results.nodes_searched / id_results.time_searched;
    return id_results;
}
