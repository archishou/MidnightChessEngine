//
// Created by Archishmaan Peyyety on 4/15/23.
//
#include "types.h"
std::ostream& operator<<(std::ostream& os, const Line& line) {
	for(const Move & i : line) {
		if (i == Move()) break;
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
	os << "Q Nodes Searched: " << results.q_nodes_searched << std::endl;
	return os;
}
