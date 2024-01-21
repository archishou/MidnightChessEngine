//
// Created by Archishmaan Peyyety on 4/16/23.
//
#include "pvs.h"
#include "../uci_interpreter/time_manager.h"

using enum TTNodeType;

void reset_search_data(SearchData &sdata) {
	sdata.search_completed = true;
	sdata.seldepth = 0;
	sdata.pv.table.fill({});
	sdata.pv.length.fill(0);
	sdata.time_limit = 0;
}

bool position_is_draw(Position &board, const int ply) {
	if (board.fifty_move_rule() >= 100) return true;
	return board.has_repetition(ply == 0 ? Position::THREE_FOLD : Position::TWO_FOLD);
}

void update_best_move_results(SearchData &sdata, int sub_depth, bool debug) {
	Line pv{};
	sdata.final_best_move = sdata.best_move;
	sdata.final_value = sdata.value;
	for (auto i = 0; i < sdata.pv.length[0]; i++) {
		pv[i] = sdata.pv.table[0][i];
	}
	if (!debug) return;
	std::cout << "info depth " 	<< sub_depth 			<< " seldepth " << sdata.seldepth
			  <<  " score cp " 	<< sdata.value 			<< " time " 	<< get_elapsed_time(TimeResolution::Milliseconds)
			  << " nodes " 		<< sdata.nodes_searched << " pv " 		<< pv << std::endl;
}

int scale_soft_time_limit(SearchParameters &params, SearchData& sdata, int depth) {
	if (params.soft_time_limit == params.hard_time_limit && params.soft_time_limit == 86'400'000)
		return params.soft_time_limit;
	if (depth <= 9) return params.soft_time_limit;

	Move best_move = sdata.best_move;
	auto best_move_nodes_spent = static_cast<double>(nodes_spent[best_move.from()][best_move.to()]);
	auto total_nodes_spent = static_cast<double>(sdata.nodes_searched);
	auto percent_nodes_spent = best_move_nodes_spent / total_nodes_spent;
	auto scaled_soft_time_limit = (1.5 - percent_nodes_spent) * 1.35;
	return static_cast<int>(std::min(params.soft_time_limit * scaled_soft_time_limit, static_cast<double>(params.hard_time_limit)));
}

template<Color color>
i32 q_search(SearchData &sdata, ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta) {

	t_table.prefetch(board.hash());

	if (ply >= MAX_PLY - 2) return board.evaluate<color>();

	if ((sdata.nodes_searched % 1024 == 0 && time_elapsed_exceeds(sdata.time_limit, TimeResolution::Milliseconds)) ||
		(sdata.nodes_searched > sdata.hard_node_limit && sdata.hard_node_limit != -1)) {

		sdata.search_completed = false;
		return 0;
	}

	sdata.seldepth = std::max(sdata.seldepth, ply);

	const i32 stand_pat = board.evaluate<color>();
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
	ScoredMoves scored_moves = order_moves<color, MoveGenerationType::CAPTURES>(capture_moves, board, ply, tdata);
	i32 futility = stand_pat + Q_SEARCH_FUTILITY_MARGIN;
	for (i32 move_idx = 0; move_idx < static_cast<i32>(scored_moves.size()); move_idx++) {
		const Move legal_move = select_move(scored_moves, move_idx);

		if (futility <= alpha && !static_exchange_eval<color>(board, legal_move, SEE_Q_MARGIN)) {
			alpha = std::max(alpha, futility);
			continue;
		}

		board.play<color>(legal_move);
		sdata.nodes_searched += 1;
		const auto score = -q_search<~color>(sdata, tdata, board, ply + 1, -beta, -alpha);
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
i32 pvs(SearchData &sdata, ThreadData &tdata, Position &board, i16 depth, i32 ply, i32 alpha, i32 beta, bool do_null) {

	t_table.prefetch(board.hash());

	if (ply >= MAX_PLY - 2) return board.evaluate<color>();

	if ((sdata.nodes_searched % 1024 == 0 && time_elapsed_exceeds(sdata.time_limit, TimeResolution::Milliseconds)) ||
		(sdata.nodes_searched > sdata.hard_node_limit && sdata.hard_node_limit != -1)) {

		sdata.search_completed = false;
		return 0;
	}

	init_pv(sdata.pv, ply);
	sdata.seldepth = std::max(sdata.seldepth, ply);

	if (ply > 0) {
		if (position_is_draw(board, ply)) return 0;
		alpha = std::max(alpha, -MATE_SCORE + ply);
		beta = std::min(beta, MATE_SCORE - ply);
		if (alpha >= beta) return alpha;
	}

	i32 alpha_initial = alpha;
	bool in_check = board.in_check<color>();
	bool pv_node = alpha != beta - 1;
	bool excluding_move = tdata.thread_stack[ply].excluded_move != EMPTY_MOVE;
	i32 static_eval = 0;

	if (in_check) depth++;

	if (depth == 0) {
		return q_search<color>(sdata, tdata, board, ply, alpha, beta);
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
		static_eval = board.evaluate<color>();
	}

	tdata.thread_stack[ply].static_eval = static_eval;
	bool improving = !in_check && ply >= 2 && static_eval >= tdata.thread_stack[ply - 2].static_eval;

	if (!static_eval_tt.entry_found && depth >= 4)
		depth -= 1;

	if (!in_check && !pv_node && !excluding_move) {
		if (depth < RFP_MAX_DEPTH && static_eval >= beta + RFP_MARGIN * depth) {
			return static_eval;
		}
	}

	if (!pv_node && !in_check && !excluding_move) {
		if (depth <= 3 && static_eval - 63 + 182 * depth <= alpha) {
			return q_search<color>(sdata, tdata, board, ply, alpha, beta);
		}
	}

	if (depth >= NMP_MIN_DEPTH && !in_check && !pv_node && !excluding_move && do_null && static_eval >= beta) {
		board.play_null<color>();

		tdata.thread_stack[ply].move = Move();
		i32 reduction = nmp_reduction(depth, beta, static_eval);
		i32 depth_prime = std::max(depth - reduction, 0);
		i32 null_eval = -pvs<~color>(sdata, tdata, board, depth_prime, ply + 1, -beta, -beta + 1, false);

		board.undo_null<color>();
		if (null_eval >= beta) return null_eval;
	}

	MoveList<color, MoveGenerationType::ALL> all_legal_moves(board);
	if (all_legal_moves.size() == 0) {
		if (in_check) return -(MATE_SCORE - ply);
		return 0;
	}

	ScoredMoves scored_moves = order_moves<color, MoveGenerationType::ALL>(all_legal_moves, board, ply, tdata);

	Move best_move = select_move(scored_moves, 0);
	i32 moves_played = 0;
	i32 value = NEG_INF_CHESS;
	for (i32 move_idx = 0; move_idx < static_cast<i32>(scored_moves.size()); move_idx++) {
		Move legal_move = select_move(scored_moves, move_idx);

		if (legal_move == tdata.thread_stack[ply].excluded_move) continue;

		if (late_move_prune(pv_node, move_idx, depth, improving)) break;
		if (futility_prune(static_eval, alpha, value, depth)) break;
		if (late_move_prune_quiet(pv_node, move_idx, legal_move, depth)) continue;
		if (history_prune<color>(tdata, pv_node, value, depth, legal_move)) continue;
		if (see_prune_pvs<color>(board, pv_node, depth, value, legal_move)) continue;

		i32 search_extension = singular_extension<color>(sdata, tdata, board, excluding_move, depth, ply,
														 alpha, beta, legal_move, tt_probe_results);

		board.play<color>(legal_move);
		moves_played += 1;
		sdata.nodes_searched += 1;
		u64 nodes_before_search = sdata.nodes_searched;

		i32 new_value;
		tdata.thread_stack[ply].move = legal_move;
		i32 search_depth = std::max(depth + search_extension, 0);

		i32 reduction = lmr_reduction(pv_node, ply, in_check, improving, move_idx, depth, legal_move);

		if (pv_node && move_idx == 0) {
			new_value = -pvs<~color>(sdata, tdata, board, search_depth - 1, ply + 1, -beta, -alpha, false);
		} else {
			new_value = -pvs<~color>(sdata, tdata, board, search_depth - reduction - 1,
									 ply + 1, -alpha - 1, -alpha, true);

			if (new_value > alpha && reduction > 0)
				new_value = -pvs<~color>(sdata, tdata, board, search_depth - 1,
										 ply + 1, -alpha - 1, -alpha, true);

			if (new_value > alpha && new_value < beta)
				new_value = -pvs<~color>(sdata, tdata, board, search_depth - 1,
										 ply + 1, -beta, -alpha, false);
		}

		board.undo<color>(legal_move);
		if (!sdata.search_completed) return 0;
		if (ply == 0 && tdata.tidx == 0) {
			u64 node_diff = sdata.nodes_searched - nodes_before_search;
			nodes_spent[legal_move.from()][legal_move.to()] += node_diff;
		}

		if (new_value > value) {
			value = new_value;

			update_pv(sdata.pv, ply, legal_move);
			best_move = legal_move;

			if (value > alpha) {
				alpha = value;
				update_history<color>(tdata, board, scored_moves, best_move, depth, move_idx, ply);
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
void aspiration_windows(SearchData &sdata, ThreadData &tdata, Position &board, i32 prev_score, i16 depth, i32 time_limit) {
	reset_search_data(sdata);
	sdata.time_limit = time_limit;
	i32 alpha = NEG_INF_CHESS;
	i32 beta = POS_INF_CHESS;
	i32 delta = ASP_WINDOW_INIT_DELTA;

	if (depth > ASP_WINDOW_MIN_DEPTH) {
		alpha = std::max(prev_score - ASP_WINDOW_INIT_WINDOW, NEG_INF_CHESS);
		beta  = std::min(prev_score + ASP_WINDOW_INIT_WINDOW, POS_INF_CHESS);
	}

	while (true) {
		if (alpha < -ASP_WINDOW_FULL_SEARCH_BOUNDS) alpha = NEG_INF_CHESS;
		if (beta  > ASP_WINDOW_FULL_SEARCH_BOUNDS)  beta  = POS_INF_CHESS;

		sdata.value = pvs<color>(sdata, tdata, board, depth, 0, alpha, beta, false);
		i32 score = sdata.value;
		if (score <= alpha) {
			alpha = std::max(alpha - delta, NEG_INF_CHESS);
			beta = (alpha + 3 * beta) / 4;
		} else if (score >= beta) {
			beta = std::min(beta + delta, POS_INF_CHESS);
			depth = std::max(depth - 1, 1);
		} else break;
		delta += delta * 2 / 3;
	}
	sdata.best_move = sdata.pv.table[0][0];
}

template<Color color>
void iterative_deepening(SearchData &sdata, ThreadData &tdata, Position &board, SearchParameters &params) {
	reset_clock();
	std::memset(nodes_spent.data(), 0, sizeof(nodes_spent));
	sdata.nodes_searched = 0;
	for (i32 sub_depth = 1; sub_depth <= params.depth; sub_depth++) {
		i32 soft_limit = scale_soft_time_limit(params, sdata, sub_depth);
		if (time_elapsed_exceeds(soft_limit, TimeResolution::Milliseconds) ||
			(sdata.nodes_searched > sdata.soft_node_limit && sdata.soft_node_limit != -1)) {
			break;
		}
		aspiration_windows<color>(sdata, tdata, board, sdata.value, sub_depth, params.hard_time_limit);
		if (sdata.search_completed) {
			update_best_move_results(sdata, sub_depth, params.debug_info);
		}
	}
}

void search(SearchData &sdata, ThreadData &tdata, Position &board, SearchParameters &parameters) {
	if (board.turn() == BLACK) iterative_deepening<BLACK>(sdata, tdata, board, parameters);
	else iterative_deepening<WHITE>(sdata, tdata, board, parameters);
}

template i32 q_search<WHITE>(SearchData &sdata, ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta);
template i32 q_search<BLACK>(SearchData &sdata, ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta);

template i32 pvs<WHITE>(SearchData &sdata, ThreadData &tdata, Position &board, i16 depth,
						i32 ply, i32 alpha, i32 beta, bool do_null);
template i32 pvs<BLACK>(SearchData &sdata, ThreadData &tdata, Position &board, i16 depth,
						i32 ply, i32 alpha, i32 beta, bool do_null);

template void
aspiration_windows<WHITE>(SearchData &sdata, ThreadData &tdata, Position &board,
						  i32 prev_score, i16 depth, i32 time_limit);
template void
aspiration_windows<BLACK>(SearchData &sdata, ThreadData &tdata, Position &board,
						  i32 prev_score, i16 depth, i32 time_limit);

template void
iterative_deepening<WHITE>(SearchData &sdata, ThreadData &tdata, Position &board, SearchParameters &params);
template void
iterative_deepening<BLACK>(SearchData &sdata, ThreadData &tdata, Position &board, SearchParameters &params);
