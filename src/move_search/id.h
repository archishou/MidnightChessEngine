//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "position.h"
#include "alphabeta.h"

struct IDResults {
    Move bestMove;
    int depthSearched;
    double timeSearched;
};

template<Color color>
IDResults iterative_deepening(Position& board, int maxTimeMilliseconds) {
    struct IDResults id_results;
    std::chrono::time_point target_end_time = std::chrono::system_clock::now() +
			std::chrono::milliseconds(maxTimeMilliseconds);

    std::clock_t start;
    start = std::clock();

    for (int subDepth = 1; subDepth <= DEPTH; subDepth++) {
        struct AlphaBetaResults ab_results =
				alpha_beta_root<color, std::chrono::system_clock>(board, subDepth, target_end_time);
        if (ab_results.search_completed) {
			id_results.bestMove = ab_results.best_move;
			id_results.depthSearched = subDepth;
        }
    }

	id_results.timeSearched = (std::clock() - start ) / (double) CLOCKS_PER_SEC;
    return id_results;
}
