//
// Created by Archishmaan Peyyety on 3/18/23.
//

#include <cmath>
#include "lmr_table.h"

array<array<array<double, MAX_DEPTH + 1>, MAX_DEPTH + 1>, 2> lmr_table;

void init_lmr_table() {
	for (int depth = 0; depth < MAX_DEPTH; depth++) {
		for (int move_idx = 0; move_idx < MAX_DEPTH; move_idx++) {
			lmr_table[0][depth][move_idx] = LMR_BASE_CAPTURE + log(depth) * log(move_idx) / LMR_DIVISOR_CAPTURE;
			lmr_table[1][depth][move_idx] = LMR_BASE_QUIET + log(depth) * log(move_idx) / LMR_DIVISOR_QUIET;
		}
	}
}
