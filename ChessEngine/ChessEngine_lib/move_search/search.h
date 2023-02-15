//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "move_generation/position.h"

#include "move_generation/position.h"
#include "alphabeta.h"
#include "constants.h"

typedef Move Line[MAX_DEPTH];
const int LINE_SIZE = MAX_DEPTH;

struct BestMoveSearchResults {
	Move best_move = Move();
	Line pv;
	int depth_searched = 0;
	int seldepth = 0;

	double time_searched = 0;
	int value = 0;

	uint64_t q_nodes_searched = 0;
	uint64_t nodes_searched = 0;
	double nodes_per_second = 0;

	int tt_key_collisions = 0;
	int nodes_in_transposition_table = 0;
};

std::ostream& operator<<(std::ostream& os, const Line& line) {
	for(const Move & i : line) {
		if (i == 0) break;
		os << i << " ";
	}
	return os;
}

bool lines_equal(Line& line_1, Line& line_2) {
	for (int i = 0; i < LINE_SIZE; ++i) {
		if (line_1[i] != line_2[i]) return false;
	}
	return true;
}

int line_size(Line& line) {
	int size = 0;
	for (Move m : line) {
		if (m == Move()) break;
		size += 1;
	}
	return size;
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
	os << "Transpositions: " << results.nodes_in_transposition_table << std::endl;
	return os;
}

void update_best_move_results(BestMoveSearchResults& search_results, AlphaBetaData& ab_results, int sub_depth, bool debug) {
	search_results.value = ab_results.value;
	search_results.best_move = ab_results.best_move;
	search_results.depth_searched = sub_depth;
	search_results.nodes_searched += ab_results.nodes_searched;
	search_results.q_nodes_searched += ab_results.q_nodes_searched;
	search_results.seldepth = ab_results.seldepth;
	search_results.tt_key_collisions = ab_results.tt_key_collisions;
	search_results.nodes_in_transposition_table = ab_results.nodes_in_transposition_table;
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
BestMoveSearchResults iterative_deepening(Position& board, int time_limit, short depth, bool debug) {
	struct BestMoveSearchResults search_results;

	reset_clock();
	TranspositionTable t_table = TranspositionTable();
	if (debug) {
		std::cout << UCI_DEBUG_STRING << "Time to initialize transposition table: " << get_elapsed_time(Milliseconds) << std::endl;
	}
	reset_clock();

	for (int sub_depth = 1; sub_depth <= depth; sub_depth++) {
		if (time_elapsed_exceeds(time_limit, Milliseconds)) {
			break;
		}
		struct AlphaBetaData ab_results = alpha_beta_root<color>(board, sub_depth, time_limit, t_table);
		if (ab_results.search_completed) {
			std::memset(search_results.pv, 0, sizeof(search_results.pv));
			update_best_move_results(search_results, ab_results, sub_depth, debug);
		}
	}

	search_results.time_searched = get_elapsed_time(Seconds);
	search_results.nodes_per_second = search_results.nodes_searched / search_results.time_searched;

	return search_results;
}

template<Color color>
BestMoveSearchResults best_move(Position& board, const BestMoveSearchParameters& parameters) {
	return iterative_deepening<color>(board, parameters.time_limit, parameters.depth, parameters.debug_info);
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

