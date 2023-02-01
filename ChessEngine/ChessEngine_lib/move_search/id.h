//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "clock.h"

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

	uint64_t nodes_searched;
	uint64_t q_nodes_searched;
	double nodes_per_second;
	int seldepth;

	int tt_key_collisions;
	int nodes_in_transposition_table;
};

void update_id_results(IDResults& id_results, AlphaBetaData& ab_results, int sub_depth) {
	std::cout << "SEARCH COMPLETED " << sub_depth << std::endl;
	id_results.value = ab_results.value;
	id_results.best_move = ab_results.best_move;
	id_results.depth_searched = sub_depth;
	id_results.nodes_searched += ab_results.nodes_searched;
	id_results.q_nodes_searched += ab_results.q_nodes_searched;
	id_results.seldepth = ab_results.seldepth;
	id_results.tt_key_collisions = ab_results.tt_key_collisions;
	id_results.nodes_in_transposition_table = ab_results.nodes_in_transposition_table;
	for (int i = 0; i < ab_results.pv.length[0]; i++) {
		id_results.pv[i] = ab_results.pv.table[0][i];
	}
}

template<Color color>
IDResults iterative_deepening(Position& board, int time_limit, int depth) {
    struct IDResults id_results;
	id_results.nodes_searched = 0;
	id_results.q_nodes_searched = 0;
	std::chrono::time_point start_time = std::chrono::system_clock::now();
    std::chrono::time_point target_end_time = start_time + std::chrono::milliseconds(time_limit);

    std::clock_t start = std::clock();

	std::cout << "Game Clock Measured Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() << std::endl;
	TranspositionTable t_table = TranspositionTable();

	std::cout << "Search Time limit: " << time_limit << std::endl;
	std::cout << "Game Clock Measured Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() << std::endl;
	std::cout << "Clock 1 Measured Time: " <<(std::clock() - start ) / (double) CLOCKS_PER_SEC  << std::endl;
	std::cout << "Exceeded time after init? " << exceeded_time(target_end_time) << std::endl;

    for (int sub_depth = 1; sub_depth <= depth; sub_depth++) {
		if (exceeded_time(target_end_time)) {
			std::cout << "LEAVING Because Exceeding Time" << std::endl;
			break;
		}
        struct AlphaBetaData ab_results =
				alpha_beta_root<color>(board, sub_depth, target_end_time, t_table);
        if (ab_results.search_completed) {
			update_id_results(id_results, ab_results, sub_depth);
        }
    }

	id_results.time_searched = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count();
	id_results.nodes_per_second = id_results.nodes_searched / id_results.time_searched;

    return id_results;
}
