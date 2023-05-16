#pragma once
#include "search_params.h"
#include "search_constants.h"
#include "move_search/tables/history_table.h"

inline bool late_move_prune(bool pv_node, int move_idx, int depth) {
	return !pv_node && depth <= LMP_MIN_DEPTH && move_idx > depth * LMP_DEPTH_MULTIPLIER;
}

inline bool late_move_prune_quiet(bool pv_node, int move_idx, Move legal_move, int depth) {
	return !pv_node && depth <= 6 && legal_move.is_quiet() && move_idx > depth * 9;
}

inline bool futility_prune(int static_eval, int alpha, int value, int depth) {
	int fp_margin = depth * FP_COEFFICIENT + FP_MARGIN;
	return value > -MATE_BOUND && depth < FP_DEPTH && static_eval + fp_margin <= alpha;
}

template<Color color>
inline bool history_prune(bool pv_node, int value, int depth, Move legal_move) {
	return !pv_node && value > -MATE_BOUND && depth < 3 && history[color][legal_move.from()][legal_move.to()] < -1024 * depth;
}

template<Color color>
inline bool see_prune_pvs(Position& board, bool pv_node, int depth, int value, Move legal_move) {
	return !pv_node && depth < SEE_PVS_MIN_DEPTH && value > -MATE_BOUND &&
	!static_exchange_eval<color>(board, legal_move, legal_move.is_quiet() ? SEE_PVS_QUIET_MARGIN * depth : SEE_PVS_TACTICAL_MARGIN * depth);
}