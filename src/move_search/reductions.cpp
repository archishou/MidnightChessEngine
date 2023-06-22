//
// Created by Archishmaan Peyyety on 3/22/23.
//

#include "reductions.h"
#include "tables/lmr_table.h"
#include "../move_gen/types/move.h"
#include <algorithm>

i32 lmr_reduction(bool pv_node, i32 ply, bool in_check, bool improving, i32 move_idx,
				  i32 depth, Move legal_move) {
	i32 reduction = 0;
	i32 lmr_depth = (pv_node || ply == 0) ? 5 : 3;
	if (depth >= 3 && move_idx > lmr_depth && !in_check) {
		reduction = static_cast<int>(lmr_table[legal_move.is_quiet()][depth][move_idx]);
		reduction -= pv_node;
		reduction += !improving;
		reduction = std::clamp(reduction, 0, depth - 1);
	}
	return reduction;
}

i32 nmp_reduction(i32 depth, i32 beta, i32 static_eval) {
	auto reduction = 3 + depth / 3 + std::min((static_eval - beta) / 200, 3);
	return std::max(reduction, 3);
}
