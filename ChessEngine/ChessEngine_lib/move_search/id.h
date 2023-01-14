//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#include "move_generation/position.h"
#include "alphabeta.h"

struct IDResults {
    Move best_move;
    int depth_searched;
    double time_searched;
	int value;
};

void update_id_results(IDResults& id_results, AlphaBetaResults& ab_results, int sub_depth) {
	id_results.value = ab_results.value;
	id_results.best_move = ab_results.best_move;
	id_results.depth_searched = sub_depth;
}

template<Color color>
IDResults iterative_deepening(Position& board, int time_limit, int depth) {
    struct IDResults id_results;
    std::chrono::time_point target_end_time = std::chrono::system_clock::now() +
			std::chrono::milliseconds(time_limit);

    std::clock_t start;
    start = std::clock();

    for (int sub_depth = 1; sub_depth <= depth; sub_depth++) {
        struct AlphaBetaResults ab_results =
				alpha_beta_root<color>(board, sub_depth, target_end_time);
        if (ab_results.search_completed) {
			update_id_results(id_results, ab_results, sub_depth);
        }
    }

	id_results.time_searched = (std::clock() - start ) / (double) CLOCKS_PER_SEC;
    return id_results;
}
