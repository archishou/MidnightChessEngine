//
// Created by Archishmaan on 12/21/22.
//


#include "move_generation/position.h"
#include "piece_square_tables.h"
#include "bitset"

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
const int CHECK_PENALTY = -100;
//const int OPP_CHECK_BONUS = 100;

template<Color color>
int evaluate_material(Position& board) {
	// TODO: better ways to handle mate, this is not ideal and can result in mis-evaluations of a position.
	int usPawns = pop_count(board.bitboard_of(color, PAWN));
	int usKnight = pop_count(board.bitboard_of(color, KNIGHT));
	int usBishop = pop_count(board.bitboard_of(color, BISHOP));
	int usRook = pop_count(board.bitboard_of(color, ROOK));
	int usQueen = pop_count(board.bitboard_of(color, QUEEN));

	return (PAWN_VALUE * usPawns) + (KNIGHT_VALUE * usKnight) + (BISHOP_VALUE * usBishop) +
		(ROOK_VALUE * usRook) + (QUEEN_VALUE * usQueen);
}

template<Color color>
int evaluate_king(Position& board) {
	return board.in_check<color>() ? CHECK_PENALTY: 0;
}

template<Color color>
int evaluate_piece_position(Position& board, PieceType piece_type) {
	int pstq_score = 0;
	Bitboard piece_bitboard = board.bitboard_of(color, piece_type);
	while (piece_bitboard != 0) {
		Square square = (Square) __builtin_ctzll(piece_bitboard);
		piece_bitboard &= ~(1ULL << square);
		pstq_score += read_pstq<color>(piece_type, square);
	}
	return pstq_score;
}

template<Color color>
int evaluate_all_piece_positions(Position& board) {
	int pstq_score = 0;
	for (PieceType piece_type : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
		pstq_score += evaluate_piece_position<color>(board, piece_type);
	}
	return pstq_score;
}

template<Color color>
int evaluation_one_side(Position& board) {
	return evaluate_material<color>(board) + evaluate_king<color>(board) + evaluate_all_piece_positions<color>(board);
}

template<Color color>
int evaluate(Position& board) {
	return evaluation_one_side<color>(board) - evaluation_one_side<~color>(board);
}
