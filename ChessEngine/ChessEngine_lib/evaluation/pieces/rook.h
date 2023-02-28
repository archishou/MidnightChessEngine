#ifndef ROOK_H
#define ROOK_H

template<Color color>
constexpr Score evaluate_rooks(Position& board) {
	Score score = Score();
	Bitboard rooks = board.bitboard_of(color, ROOK);
	while (rooks) {
		Square rook_square = pop_lsb(&rooks);
		score += PIECE_VALUES[ROOK];
		score += read_psqt<color>(ROOK, rook_square);
	}
	return score;
}

#endif // ROOK_H