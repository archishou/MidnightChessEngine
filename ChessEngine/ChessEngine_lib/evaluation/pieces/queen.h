#ifndef QUEEN_H
#define QUEEN_H

template<Color color>
constexpr Score evaluate_queens(Position& board) {
	Score score = Score();
	Bitboard queens = board.bitboard_of(color, QUEEN);
	while (queens) {
		Square queen_square = pop_lsb(&queens);
		score += PIECE_VALUES[QUEEN];
		score += read_pstq<color>(QUEEN, queen_square);
	}
	return score;
}

#endif // QUEEN_H