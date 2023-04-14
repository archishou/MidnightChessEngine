//
// Created by Archishmaan Peyyety on 3/22/23.
//

#include "reductions.h"
#include "tables/lmr_table.h"
#include <algorithm>

int lmr_reduction(const bool& pv_node, const int& ply, const bool& in_check, const int& move_idx, const int& depth, const Move& legal_move) {
	int reduction = 0;
	int lmr_depth = (pv_node || ply == 0) ? 5 : 3;
	if (depth >= 3 && move_idx > lmr_depth && legal_move.is_quiet() && !in_check) {
		reduction = static_cast<int>(lmr_table[depth][move_idx]);
		reduction -= pv_node;
		reduction = std::clamp(reduction, 0, depth - 1);
	}
	return reduction;
}

int nmp_reduction(const int& depth, const int& beta, const int& static_eval) {
	int reduction = 3 + depth/3 + std::min((static_eval - beta) / 200, 3);
	return std::max(reduction, 3);
}
