#ifndef KING_H
#define KING_H

template<Color color>
constexpr Score evaluate_king(Position& board) {
	Score score = Score();
	Bitboard king = board.bitboard_of(color, KING);
	Square king_square = pop_lsb(&king);
	score += PIECE_VALUES[KING];
	score += read_pstq<color>(KING, king_square);
	return score;
}

#endif // KING_H