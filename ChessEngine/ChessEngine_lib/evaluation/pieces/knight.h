#ifndef KNIGHT_H
#define KNIGHT_H

template<Color color>
constexpr Score evaluate_knight(Position& board) {
	Score score = Score();
	Bitboard knights = board.bitboard_of(color, KNIGHT);
	while (knights) {
		Square knight_square = pop_lsb(&knights);
		score += PIECE_VALUES[KNIGHT];
		score += read_psqt<color>(KNIGHT, knight_square);
	}
	return score;
}

#endif // KNIGHT_H