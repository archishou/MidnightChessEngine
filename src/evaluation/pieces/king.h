#ifndef KING_H
#define KING_H

constexpr int NKING_SIDES = 2;
constexpr Bitboard KING_SAFE_AREA[NCOLORS] = {
		0xC3D7, 0xd7c3000000000000
};
constexpr Bitboard PAWN_SHIELD[NKING_SIDES][NCOLORS] = {
		{0x700, 0x7000000000000},
		{0xe000, 0xe0000000000000}
};

template<Color color>
constexpr Score evaluate_king(Position& board) {
	Score score = Score();
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);
	const Bitboard pawns = board.bitboard_of(color, PAWN);

	Bitboard king = board.bitboard_of(color, KING);
	Square king_square = pop_lsb(&king);
	File file = file_of(king_square);
	const int king_side = file >> 2;

	const Bitboard king_bb = SQUARE_BB[king_square];
	score += PIECE_VALUES[KING];
	score += read_psqt<color>(KING, king_square);

	const bool on_open_file = king_bb & board_open_files;
	const bool on_semi_open_file = king_bb & board_semi_open_files;
	score += (OPEN_FILE_BONUS[KING] * on_open_file) + (SEMI_OPEN_FILE_BONUS[KING] * on_semi_open_file);

	if (king_bb & KING_SAFE_AREA[color]) {
		const Bitboard pawn_shield = PAWN_SHIELD[king_side][color];
		score += KING_PAWN_SHIELD[0] * pop_count(pawns & pawn_shield);
		score += KING_PAWN_SHIELD[1] * pop_count(pawns & shift<relative_dir<color>(NORTH)>(pawn_shield));
	}
	return score;
}

#endif // KING_H