//
// Created by Archishmaan Peyyety on 4/16/23.
//
#include "pvs.h"
#include "../board/position.h"

PVSData data;

using enum TTNodeType;

void reset_data() {
	data.search_completed = true;
	data.seldepth = 0;
	data.pv.table.fill({});
	data.pv.length.fill(0);
	std::fill(data.moves_made.begin(), data.moves_made.end(), EMPTY_MOVE);
	std::fill(data.excluded_moves.begin(), data.excluded_moves.end(), EMPTY_MOVE);
	data.time_limit = 0;
}

bool position_is_draw(Position &board, const int ply) {
	if (board.fifty_move_rule() >= 100) return true;
	return board.has_repetition(ply == 0 ? Position::THREE_FOLD : Position::TWO_FOLD);
}

void update_best_move_results(int sub_depth, bool debug) {
	Line pv{};
	data.final_best_move = data.best_move;
	for (auto i = 0; i < data.pv.length[0]; i++) {
		pv[i] = data.pv.table[0][i];
	}
	if (debug) {
		std::cout	<< "info depth " << sub_depth << " seldepth " << data.seldepth
					 <<  " score cp " << data.value << " time " << get_elapsed_time(TimeResolution::Milliseconds)
					 << " nodes " << data.nodes_searched << " pv " << pv << std::endl;
	}
}

int scale_soft_time_limit(BestMoveSearchParameters &params, PVSData& ab_results, int depth) {
	if (params.soft_time_limit == params.hard_time_limit && params.soft_time_limit == 86'400'000) return params.soft_time_limit;
	if (depth <= 7) return params.soft_time_limit;

	Move best_move = ab_results.best_move;
	auto best_move_nodes_spent = static_cast<double>(ab_results.nodes_spend[best_move.from()][best_move.to()]);
	auto total_nodes_spent = static_cast<double>(data.nodes_searched);
	auto percent_nodes_spent = best_move_nodes_spent / total_nodes_spent;
	auto scaled_soft_time_limit = (1.5 - percent_nodes_spent) * 1.35;
	return static_cast<int>(std::min(params.soft_time_limit * scaled_soft_time_limit, static_cast<double>(params.hard_time_limit)));
}

template<Color color>
i32 q_search(ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta) {

	t_table.prefetch(board.hash());

	if (ply >= MAX_PLY - 2) return evaluate<color>(board);

	if (data.nodes_searched % 1024 == 0 &&
		time_elapsed_exceeds(data.time_limit, TimeResolution::Milliseconds)) {

		data.search_completed = false;
		return 0;
	}

	data.seldepth = std::max(data.seldepth, ply);

	const i32 stand_pat = evaluate<color>(board);
	if (stand_pat >= beta) return beta;
	if (alpha < stand_pat) alpha = stand_pat;

	TranspositionTableSearchResults probe_results = t_table.probe_for_search(board.hash(), QSEARCH_TT_DEPTH, ply);
	if (probe_results.entry_found) {
		if ((probe_results.entry.node_type == EXACT) ||
			(probe_results.entry.node_type == LOWER_NODE && probe_results.entry.value >= beta) ||
			(probe_results.entry.node_type == UPPER_NODE && probe_results.entry.value <= alpha)) {
			return probe_results.entry.value;
		}
	}

	auto best_move = EMPTY_MOVE;
	MoveList<color, MoveGenerationType::CAPTURES> capture_moves(board);
	ScoredMoves scored_moves = order_moves<color, MoveGenerationType::CAPTURES>(capture_moves, board, ply, data, tdata);
	i32 futility = stand_pat + Q_SEARCH_FUTILITY_MARGIN;
	for (i32 move_idx = 0; move_idx < static_cast<i32>(scored_moves.size()); move_idx++) {
		const Move legal_move = select_move(scored_moves, move_idx);

		if (futility <= alpha && !static_exchange_eval<color>(board, legal_move, SEE_Q_MARGIN)) {
			alpha = std::max(alpha, futility);
			continue;
		}

		board.play<color>(legal_move);
		data.nodes_searched += 1;
		const auto score = -q_search<~color>(tdata, board, ply + 1, -beta, -alpha);
		board.undo<color>(legal_move);

		if (score >= beta) {
			alpha = beta;
			best_move = legal_move;
			break;
		}
		if (score > alpha) {
			best_move = legal_move;
			alpha = score;
		}
	}
	TTNodeType node_type{};
	if (alpha >= beta) node_type = LOWER_NODE;
	else node_type = UPPER_NODE;
	t_table.put(board.hash(), QSEARCH_TT_DEPTH, alpha, ply, best_move, QSEARCH_TT_PV_NODE, node_type);
	return alpha;
}

template<Color color>
i32 pvs(ThreadData &tdata, Position &board, i16 depth, i32 ply, i32 alpha, i32 beta, bool do_null) {

	t_table.prefetch(board.hash());

	if (ply >= MAX_PLY - 2) return evaluate<color>(board);

	if (data.nodes_searched % 1024 == 0 &&
		time_elapsed_exceeds(data.time_limit, TimeResolution::Milliseconds)) {

		data.search_completed = false;
		return 0;
	}

	init_pv(data.pv, ply);
	data.seldepth = std::max(data.seldepth, ply);

	if (ply > 0) {
		if (position_is_draw(board, ply)) return 0;
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) return alpha;
	}

	i32 alpha_initial = alpha;
	bool in_check = board.in_check<color>();
	bool pv_node = alpha != beta - 1;
	bool excluding_move = data.excluded_moves[ply] != EMPTY_MOVE;
	i32 static_eval = 0;

	if (in_check) depth++;

	if (depth == 0) {
		return q_search<color>(tdata, board, ply, alpha, beta);
	}

	TranspositionTableSearchResults tt_probe_results = t_table.probe_for_search(board.hash(), depth, ply);
	if (tt_probe_results.entry_found && !pv_node && !excluding_move) {
		TranspositionTableEntry tt_entry = tt_probe_results.entry;
		if (tt_entry.node_type == EXACT) {
			return tt_entry.value;
		} else if (tt_entry.node_type == LOWER_NODE) {
			alpha = std::max(alpha, tt_entry.value);
		} else if (tt_entry.node_type == UPPER_NODE) {
			beta = std::min(beta, tt_entry.value);
		}
		if (alpha >= beta) {
			return tt_entry.value;
		}
	}

	TranspositionTableSearchResults static_eval_tt = t_table.probe_eval(board.hash(), ply);
	if (static_eval_tt.entry_found) {
		static_eval = static_eval_tt.entry.value;
	} else {
		static_eval = evaluate<color>(board);
	}

	data.evals[ply] = static_eval;
	bool improving = !in_check && ply >= 2 && static_eval >= data.evals[ply - 2];

	if (!static_eval_tt.entry_found && depth >= 4)
		depth -= 1;

	if (!in_check && !pv_node && !excluding_move) {
		if (depth < RFP_MAX_DEPTH && static_eval >= beta + RFP_MARGIN * depth) {
			return static_eval;
		}
	}

	if (!pv_node && !in_check && !excluding_move) {
		if (depth <= 3 && static_eval - 63 + 182 * depth <= alpha) {
			return q_search<color>(tdata, board, ply, alpha, beta);
		}
	}

	if (depth >= NMP_MIN_DEPTH && !in_check && !pv_node && !excluding_move && do_null && static_eval >= beta) {
		board.play_null<color>();

		data.moves_made[ply] = Move();
		i32 reduction = nmp_reduction(depth, beta, static_eval);
		i32 depth_prime = std::max(depth - reduction, 0);
		i32 null_eval = -pvs<~color>(tdata, board, depth_prime, ply + 1, -beta, -beta + 1, false);

		board.undo_null<color>();
		if (null_eval >= beta) return null_eval;
	}

	MoveList<color, MoveGenerationType::ALL> all_legal_moves(board);
	if (all_legal_moves.size() == 0) {
		if (in_check) return -(MATE_SCORE - ply);
		return 0;
	}

	ScoredMoves scored_moves = order_moves<color, MoveGenerationType::ALL>(all_legal_moves, board, ply, data, tdata);

	Move best_move = select_move(scored_moves, 0);
	i32 moves_played = 0;
	i32 value = NEG_INF_CHESS;
	for (i32 move_idx = 0; move_idx < static_cast<i32>(scored_moves.size()); move_idx++) {
		Move legal_move = select_move(scored_moves, move_idx);

		if (legal_move == data.excluded_moves[ply]) continue;

		if (late_move_prune(pv_node, move_idx, depth, improving)) break;
		if (futility_prune(static_eval, alpha, value, depth)) break;
		if (late_move_prune_quiet(pv_node, move_idx, legal_move, depth)) continue;
		if (history_prune<color>(tdata, pv_node, value, depth, legal_move)) continue;
		if (see_prune_pvs<color>(board, pv_node, depth, value, legal_move)) continue;

		i32 search_extension = singular_extension<color>(tdata, board, excluding_move, depth, ply, alpha, beta,
														 legal_move, tt_probe_results, data);

		board.play<color>(legal_move);
		moves_played += 1;
		data.nodes_searched += 1;
		u64 nodes_before_search = data.nodes_searched;

		i32 new_value;
		data.moves_made[ply] = legal_move;
		i32 search_depth = std::max(depth + search_extension, 0);

		i32 reduction = lmr_reduction(pv_node, ply, in_check, improving, move_idx, depth, legal_move);

		if (pv_node && move_idx == 0) {
			new_value = -pvs<~color>(tdata, board, search_depth - 1, ply + 1, -beta, -alpha, false);
		} else {
			new_value = -pvs<~color>(tdata, board, search_depth - reduction - 1, ply + 1, -alpha - 1, -alpha, true);

			if (new_value > alpha && reduction > 0)
				new_value = -pvs<~color>(tdata, board, search_depth - 1, ply + 1, -alpha - 1, -alpha, true);

			if (new_value > alpha && new_value < beta)
				new_value = -pvs<~color>(tdata, board, search_depth - 1, ply + 1, -beta, -alpha, false);
		}

		board.undo<color>(legal_move);
		if (!data.search_completed) return 0;
		if (ply == 0) {
			u64 node_diff = data.nodes_searched - nodes_before_search;
			data.nodes_spend[legal_move.from()][legal_move.to()] += node_diff;
		}

		if (new_value > value) {
			value = new_value;

			update_pv(data.pv, ply, legal_move);
			best_move = legal_move;

			if (value > alpha) {
				alpha = value;
				update_history<color>(tdata, board, scored_moves, best_move, depth, move_idx, ply, data);
				if (alpha >= beta) {
					update_killers(tdata, best_move, ply);
					break;
				}
			}
		}
	}
	if (moves_played == 0) {
		if (excluding_move) return alpha;
		else {
			std::cout << "No Moves Played! Panic Exit" << std::endl;
			exit(1);
		}
	}
	TTNodeType node_type = t_table.get_node_type(alpha_initial, beta, value);
	t_table.put(board.hash(), depth, value, ply, best_move, pv_node, node_type);
	return value;
}

template<Color color>
PVSData aspiration_windows(ThreadData &tdata, Position &board, i32 prev_score, i16 depth, i32 time_limit) {
	reset_data();
	data.time_limit = time_limit;
	i32 alpha = NEG_INF_CHESS;
	i32 beta = POS_INF_CHESS;
	i32 delta = ASP_WINDOW_INIT_DELTA;

	if (depth > ASP_WINDOW_MIN_DEPTH) {
		alpha = std::max(prev_score - ASP_WINDOW_INIT_WINDOW, NEG_INF_CHESS);
		beta  = std::min(prev_score + ASP_WINDOW_INIT_WINDOW, POS_INF_CHESS);
	}

	while (true) {
		if (alpha < -ASP_WINDOW_FULL_SEARCH_BOUNDS) alpha = NEG_INF_CHESS;
		if (beta  > ASP_WINDOW_FULL_SEARCH_BOUNDS) beta  = POS_INF_CHESS;

		data.value = pvs<color>(tdata, board, depth, 0, alpha, beta, false);
		i32 score = data.value;
		if (score <= alpha) {
			alpha = std::max(alpha - delta, NEG_INF_CHESS);
			beta = (alpha + 3 * beta) / 4;
		} else if (score >= beta) {
			beta = std::min(beta + delta, POS_INF_CHESS);
			depth = std::max(depth - 1, 1);
		} else break;
		delta += delta * 2 / 3;
	}
	data.best_move = data.pv.table[0][0];
	return data;
}

template<Color color>
void iterative_deepening(ThreadData &tdata, Position &board, BestMoveSearchParameters &params) {
	reset_clock();
	std::memset(data.nodes_spend.data(), 0, sizeof(data.nodes_spend));
	data.nodes_searched = 0;
	for (i32 sub_depth = 1; sub_depth <= params.depth; sub_depth++) {
		i32 soft_limit = scale_soft_time_limit(params, data, sub_depth);
		if (time_elapsed_exceeds(soft_limit, TimeResolution::Milliseconds)) {
			break;
		}
		aspiration_windows<color>(tdata, board, data.value, sub_depth, params.hard_time_limit);
		if (data.search_completed) {
			update_best_move_results(sub_depth, params.debug_info);
		}
	}
}

void search(ThreadData &tdata, Position &board, BestMoveSearchParameters &parameters) {
	if (board.turn() == BLACK) iterative_deepening<BLACK>(tdata, board, parameters);
	else iterative_deepening<WHITE>(tdata, board, parameters);
}

template i32 q_search<WHITE>(ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta);
template i32 q_search<BLACK>(ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta);

template i32 pvs<WHITE>(ThreadData &tdata, Position &board, i16 depth, i32 ply, i32 alpha, i32 beta, bool do_null);
template i32 pvs<BLACK>(ThreadData &tdata, Position &board, i16 depth, i32 ply, i32 alpha, i32 beta, bool do_null);

template PVSData
aspiration_windows<WHITE>(ThreadData &tdata, Position &board, i32 prev_score, i16 depth, i32 time_limit);
template PVSData
aspiration_windows<BLACK>(ThreadData &tdata, Position &board, i32 prev_score, i16 depth, i32 time_limit);

template void iterative_deepening<WHITE>(ThreadData &tdata, Position &board, BestMoveSearchParameters &params);
template void iterative_deepening<BLACK>(ThreadData &tdata, Position &board, BestMoveSearchParameters &params);
