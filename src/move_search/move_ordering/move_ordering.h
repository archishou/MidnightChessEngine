//
// Created by Archishmaan Peyyety on 1/8/23.
//
#pragma once
#include <algorithm>
#include "move_generation/position.h"
#include "move_generation/tables.h"
#include "move_search/tables/transposition_table.h"
#include "move_search/tables/history_table.h"
#include "move_search/types.h"

void initialize_move_sort_tables();

int get_piece_value(PieceType piece_type);

int hash_move_score(Move& move, Move& previous_best_move);

int capture_move_score(Move move, Position& board);

int promotion_move_score(Move move);

template<Color color>
bool static_exchange_eval(Position& board, Move move, const int threshold) {

	if (move.is_promotion()) return true;

	Square to = move.to();
	Square from = move.from();
	PieceType piece_captured = type_of(board.at(to));
	PieceType piece_capturing = type_of(board.at(from));

	// If we make the capture and don't loose our piece, we should beat the threshold.
	// If we don't it's likely a bad exchange.
	int value = ORDERING_PIECE_VALUES[piece_captured] - threshold;
	if (value < 0) return false;

	// If we loose our piece and are still positive, this is a good exchange.
	value -= ORDERING_PIECE_VALUES[piece_capturing];
	if (value >= 0) return true;

	// We already know the piece on 'from' is an attacker. Ignore it. The piece on to will be captured. Ignore it.
	Bitboard occupied = board.all_pieces() ^ SQUARE_BB[from] ^ SQUARE_BB[to];
	// This will be updated to only contain pieces that only have pieces we are concerned with.
	Bitboard attackers = board.attackers_from(to, occupied);

	Bitboard bishops = board.all_pieces(BISHOP) | board.all_pieces(QUEEN);
	Bitboard rooks   = board.all_pieces(ROOK)   | board.all_pieces(QUEEN);

	Color side_to_play = ~color;

	while (true) {
		attackers &= occupied;

		Bitboard our_attackers = attackers & board.all_pieces(side_to_play);
		if (!our_attackers) break; // If no remaining attackers for us, break.

		int ipt;
		for (ipt = PAWN; ipt < KING; ipt++) { // pt == king when we break if all others didn't cause break.
			if (our_attackers & board.bitboard_of(side_to_play, PieceType(ipt))) break;
		}
		auto piece_type = PieceType(ipt);

		side_to_play = ~side_to_play;

		value = -value - 1 - ORDERING_PIECE_VALUES[piece_type];
		if (value >= 0) {
			if (piece_type == KING && (attackers & board.all_pieces(side_to_play))) side_to_play = ~side_to_play;
			break;
		}

		occupied ^= SQUARE_BB[bsf(our_attackers & board.all_pieces(piece_type))];

		// Add discovered attacks.
		if (piece_type == PAWN || piece_type == BISHOP || piece_type == QUEEN)
			attackers |= attacks<BISHOP>(to, occupied) & bishops;
		if (piece_type == ROOK || piece_type == QUEEN)
			attackers |= attacks<ROOK>(to, occupied) & rooks;
	}
	return side_to_play != color_of(board.at(from));
}

template<Color color>
int capture_move_score(Move move, Position& board) {
	if (!move.is_capture()) return 0;
	PieceType to_type = type_of(board.at(move.to()));
	PieceType from_type = type_of(board.at(move.from()));
	return MVV_LVA_BONUS * static_exchange_eval<color>(board, move, -107) +
	        get_piece_value(to_type) - get_piece_value(from_type);
}

template<Color color>
int history_score(Move &move, int ply) {
	if (!move.is_quiet()) return 0;
	if (move == killers[ply][0]) return KILLER_MOVE_BONUS + 2000;
	else if (move == killers[ply][1]) return KILLER_MOVE_BONUS + 1000;
	return history[color][move.from()][move.to()];
}

template<Color color>
int in_opponent_pawn_territory(Move move, Position& board) {
	Bitboard opp_pawn_attacks = pawn_attacks<~color>(board.bitboard_of(~color, PAWN));
	if ((1ULL << move.to()) & opp_pawn_attacks) return IN_OPP_PAWN_TERRITORY_PENALTY;
	return 0;
}

template<Color color, MoveGenType move_gen_type>
ScoredMoves order_moves(MoveList<color, move_gen_type>& move_list, Position& board, int ply) {
	ScoredMoves scored_moves;
	scored_moves.reserve(move_list.size());
	Move previous_best_move = Move();
	TranspositionTableSearchResults search_results = t_table.probe_for_move_ordering(board.get_hash());
	if (search_results.entry_found) previous_best_move = search_results.entry.best_move;
	for (Move move : move_list) {
		struct ScoredMove scored_move;
		scored_move.move = move;
		int score = 0; //Higher score is likely a better move.
		score += hash_move_score(move, previous_best_move);
		score += capture_move_score<color>(move, board);
		score += promotion_move_score(move);
		score += history_score<color>(move, ply);
		score += in_opponent_pawn_territory<color>(move, board);
		// Score negated for sorting. We want to evaluate high scoring moves first.
		scored_move.score = -score;
		scored_moves.push_back(scored_move);
	}
	return scored_moves;
}

Move& select_move(ScoredMoves& scored_moves, int idx);
