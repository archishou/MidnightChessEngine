//
// Created by Archishmaan Peyyety on 1/8/23.
//
#pragma once
#include <algorithm>
#include "../../board/types/piece.h"
#include "../../move_gen/types/move.h"
#include "../../board/position.h"
#include "ordering_constants.h"
#include "../tables/history_table.h"
#include "../tables/transposition_table.h"
#include "../../move_gen/move_generator.h"

int get_piece_value(PieceType piece_type);

int hash_move_score(Move& move, Move& previous_best_move);

int capture_move_score(Move move, Position &board, ThreadData &tdata);

int promotion_move_score(Move move);

template<Color color>
bool static_exchange_eval(Position& board, Move move, const int threshold) {

	if (move.is_promotion()) return true;

	Square to = move.to();
	Square from = move.from();
	PieceType piece_captured = type_of(board.piece_at(to));
	PieceType piece_capturing = type_of(board.piece_at(from));

	// If we make the capture and don't loose our piece, we should beat the threshold.
	// If we don't it's likely a bad exchange.
	int value = ORDERING_PIECE_VALUES[piece_captured] - threshold;
	if (value < 0) return false;

	// If we loose our piece and are still positive, this is a good exchange.
	value -= ORDERING_PIECE_VALUES[piece_capturing];
	if (value >= 0) return true;

	// We already know the piece on 'from' is an attacker. Ignore it. The piece on to will be captured. Ignore it.
	Bitboard occupied = board.occupancy() ^ square_to_bitboard(from) ^ square_to_bitboard(to);
	// This will be updated to only contain pieces that only have pieces we are concerned with.
	Bitboard attackers = board.attackers_of(to, occupied);

	Bitboard bishops = board.occupancy<BISHOP>() | board.occupancy<QUEEN>();
	Bitboard rooks   = board.occupancy<ROOK>()   | board.occupancy<QUEEN>();

	Color side_to_play = ~color;

	while (true) {
		attackers &= occupied;

		Bitboard our_attackers = attackers & board.occupancy(side_to_play);
		if (!our_attackers) break; // If no remaining attackers for us, break.

		u32 ipt;
		for (ipt = PAWN; ipt < KING; ipt++) { // pt == king when we break if all others didn't cause break.
			if (our_attackers & board.occupancy(side_to_play, PieceType(ipt))) break;
		}
		auto piece_type = PieceType(ipt);

		side_to_play = ~side_to_play;

		value = -value - 1 - ORDERING_PIECE_VALUES[piece_type];
		if (value >= 0) {
			if (piece_type == KING && (attackers & board.occupancy(side_to_play))) side_to_play = ~side_to_play;
			break;
		}

		occupied ^= square_to_bitboard(lsb(our_attackers & board.occupancy(piece_type)));

		// Add discovered attacks.
		if (piece_type == PAWN || piece_type == BISHOP || piece_type == QUEEN)
			attackers |= tables::attacks<BISHOP>(to, occupied) & bishops;
		if (piece_type == ROOK || piece_type == QUEEN)
			attackers |= tables::attacks<ROOK>(to, occupied) & rooks;
	}
	return side_to_play != color_of(board.piece_at(from));
}

template<Color color>
int capture_move_score(Move move, Position &board, ThreadData &tdata) {
	if (!move.is_capture()) return 0;
	PieceType to_type = type_of(board.piece_at(move.to()));
	PieceType from_type = type_of(board.piece_at(move.from()));
	int cap_hist_score = tdata.capture_history[board.piece_at(move.from())][move.to()][board.piece_at(move.to())];
	return (MVV_LVA_BONUS + cap_hist_score) * static_exchange_eval<color>(board, move, -107) +
	        get_piece_value(to_type) - get_piece_value(from_type);
}

template<Color color>
int history_score(Move &move, int ply, Position &board, PVSData &data, ThreadData &tdata) {
	if (!move.is_quiet()) return 0;
	if (move == tdata.killers[ply][0]) return KILLER_MOVE_BONUS + 2000;
	else if (move == tdata.killers[ply][1]) return KILLER_MOVE_BONUS + 1000;

	Move one_mv_ago = ply > 0 ? data.moves_made[ply - 1] : Move();
	Move two_mv_ago = ply > 1 ? data.moves_made[ply - 2] : Move();

	int one_mv_ago_score = tdata.continuation_history[board.piece_at(one_mv_ago.from())][one_mv_ago.to()][board.piece_at(move.from())][move.to()];
	one_mv_ago_score = one_mv_ago != Move() ? one_mv_ago_score : 0;

	int two_mv_ago_score = tdata.continuation_history[board.piece_at(two_mv_ago.from())][two_mv_ago.to()][board.piece_at(move.from())][move.to()];
	two_mv_ago_score = two_mv_ago != Move() ? two_mv_ago_score : 0;

	return tdata.history[color][move.from()][move.to()] + one_mv_ago_score + two_mv_ago_score;
}

template<Color color>
int in_opponent_pawn_territory(Move move, Position& board) {
	Bitboard them_pawns = board.occupancy<~color, PAWN>();
	Bitboard opp_pawn_attacks = shift_relative<color, NORTH_EAST>(them_pawns) | shift_relative<color, NORTH_WEST>(them_pawns);
	if ((1ULL << move.to()) & opp_pawn_attacks) return IN_OPP_PAWN_TERRITORY_PENALTY;
	return 0;
}

template<Color color, MoveGenerationType move_gen_type>
ScoredMoves order_moves(MoveList<color, move_gen_type>& move_list, Position& board, int ply, PVSData& data, ThreadData& tdata) {
	ScoredMoves scored_moves;
	scored_moves.reserve(move_list.size());
	Move previous_best_move = Move();
	TranspositionTableSearchResults search_results = t_table.probe_for_move_ordering(board.hash());
	if (search_results.entry_found) previous_best_move = search_results.entry.best_move;
	for (Move move : move_list) {
		ScoredMove scored_move;
		scored_move.move = move;
		int score = 0; //Higher score is likely a better move.
		score += hash_move_score(move, previous_best_move);
		score += capture_move_score<color>(move, board, tdata);
		score += promotion_move_score(move);
		score += history_score<color>(move, ply, board, data, tdata);
		score += in_opponent_pawn_territory<color>(move, board);
		// Score negated for sorting. We want to evaluate high scoring moves first.
		scored_move.score = -score;
		scored_moves.push_back(scored_move);
	}
	return scored_moves;
}

Move& select_move(ScoredMoves& scored_moves, int idx);
