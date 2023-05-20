//
// Created by Archishmaan Peyyety on 5/17/23.
//
#include "king.h"


template<Color Us, DoTrace do_trace>
Score evaluate_king(const Position& board, Trace& trace) {
	Score score = SCORE_ZERO;
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<Us>(board);
	const Bitboard pawns = board.occupancy<Us, PAWN>();

	Bitboard king = board.occupancy<Us, KING>();
	Square king_square = pop_lsb(king);
	File file = file_of(king_square);
	const int king_side = file >> 2;

	const Bitboard king_bb = square_to_bitboard(king_square);
	score += PIECE_VALUES[KING];
	score += read_psqt<Us, KING>(king_square);

	const bool on_open_file = king_bb & board_open_files;
	const bool on_semi_open_file = king_bb & board_semi_open_files;
	score += (OPEN_FILE_BONUS[KING] * on_open_file) + (SEMI_OPEN_FILE_BONUS[KING] * on_semi_open_file);

	if (king_bb & KING_SAFE_AREA[Us]) {
		const Bitboard pawn_shield = PAWN_SHIELD[king_side][Us];
		score += KING_PAWN_SHIELD[0] * pop_count(pawns & pawn_shield);
		score += KING_PAWN_SHIELD[1] * pop_count(pawns & shift_relative<Us, NORTH>(pawn_shield));
	}
	return score;
}

template Score evaluate_king<WHITE, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_king<BLACK, TRACE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_king<WHITE, COMPUTE_EVAL>(const Position& board, Trace& trace);
template Score evaluate_king<BLACK, COMPUTE_EVAL>(const Position& board, Trace& trace);