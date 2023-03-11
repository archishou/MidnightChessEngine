#ifndef KING_H
#define KING_H

template<Color color>
constexpr Score evaluate_king(Position& board) {
	Score score = Score();
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);

	Bitboard king = board.bitboard_of(color, KING);
	Square king_square = pop_lsb(&king);
	score += PIECE_VALUES[KING];
	score += read_psqt<color>(KING, king_square);

	const bool on_open_file = king & board_open_files;
	const bool on_semi_open_file = king & board_semi_open_files;
	score += (OPEN_FILE_BONUS[KING] * on_open_file) + (SEMI_OPEN_FILE_BONUS[KING] * on_semi_open_file);
	return score;
}

#endif // KING_H