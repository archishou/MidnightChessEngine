#pragma once
#include "types.h"
#include "../board/position.h"
#include "../board/types/bitboard.h"
#include "constants/constants.h"
#include "pieces/pawn.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "iostream"
#include "../move_search/tables/transposition_table.h"

template<Color color>
std::pair<Score, i32>compute_piece_count_phase(const Position& board);

SharedEvalFeatures generate_shared_eval(const Position& board);

i32 scale_eval(i32 eval, i32 piece_count);

template<Color color, DoTrace do_trace>
Score evaluate_single_side(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);

template<Color color, bool do_trace = COMPUTE_EVAL>
auto evaluate(const Position& board) {
	Trace trace{};

	const auto [material, game_phase] = compute_piece_count_phase<color>(board);
	const SharedEvalFeatures eval_features = generate_shared_eval(board);

	Score cached_pawn_score;
	auto pawn_probe_results = t_table.probe_pawn_cache(board.pawn_hash());
	if (pawn_probe_results.entry_found && pawn_probe_results.entry.zobrist_hash == board.pawn_hash()) {
		cached_pawn_score = pawn_probe_results.entry.score;
		if constexpr (color == BLACK) cached_pawn_score = -cached_pawn_score;
	} else {
		Score us_cached = evaluate_cachable_pawn_struct<color, do_trace>(board, eval_features, trace);
		Score them_cached = evaluate_cachable_pawn_struct<~color, do_trace>(board, eval_features, trace);
		cached_pawn_score = us_cached - them_cached;
		Score push_to_cache = cached_pawn_score;
		if constexpr (color == BLACK) push_to_cache = -push_to_cache;
		t_table.pawn_table_put(board.pawn_hash(), push_to_cache);
	}

	Score us = evaluate_single_side<color, do_trace>(board, eval_features, trace);
	Score them = evaluate_single_side<~color, do_trace>(board, eval_features, trace);

	Score total = cached_pawn_score + us - them + TEMPO;
	if constexpr (do_trace) trace.tempo[color] += 1;

	i32 mg_phase = std::min(game_phase, 24);
	i32 eg_phase = 24 - mg_phase;

	const i32 mg_score = mg_value(total);
	const i32 eg_score = eg_value(total);
	const i32 mg_material = mg_value(material);
	const i32 eg_material = eg_value(material);

	i32 eval = (mg_score * mg_phase + eg_score * eg_phase) / 24;
	i32 scaled_material = (mg_material * mg_phase + eg_material * eg_phase) / 24;

	if constexpr (do_trace) {
		if constexpr (color == BLACK) eval = -eval;
		trace.score = eval;
		return trace;
	} else {
		return scale_eval(eval, scaled_material);
	}
}
