#pragma once
#include "search_params.h"
#include "search_constants.h"
#include "types.h"
#include "tables/transposition_table.h"

template<Color color>
i32 singular_extension(Position& board, bool excluding_move, i32 depth, i32 ply, i32 alpha, i32 beta,
			  Move legal_move, TranspositionTableSearchResults tt_probe_results, PVSData& data) {

	i32 extension = 0;
	bool possible_singularity = !excluding_move && ply > 0 && tt_probe_results.entry_found &&
								depth >= 8 && legal_move == tt_probe_results.entry.best_move &&
								tt_probe_results.entry.depth >= depth - 3 &&
								tt_probe_results.entry.node_type != UPPER_NODE;
	if (possible_singularity) {
		data.excluded_moves[ply] = legal_move;

		i32 tt_value = tt_probe_results.entry.value;
		i32 singularity_beta = std::max(tt_value - 2 * depth, -MATE_BOUND);
		i32 singularity_depth = (depth - 1) >> 1;
		i32 singularity_score = pvs<color>(board, singularity_depth, ply, singularity_beta - 1, singularity_beta, false);

		data.excluded_moves[ply] = EMPTY_MOVE;

		if (singularity_score < singularity_beta) extension = 1;
		else if (tt_value >= beta || tt_value <= alpha) extension = -1;
		else extension = 0;
	}
	return extension;
}