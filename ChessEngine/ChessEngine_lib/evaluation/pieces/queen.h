#ifndef QUEEN_H
#define QUEEN_H

template<Color color>
constexpr int count_pseudo_legal_queen(Position& board, Square queen_square) {
	const Bitboard us_pieces = board.all_pieces<color>();
	const Bitboard them_pieces = board.all_pieces<~color>();
	Bitboard pseudo_legal_moves = attacks<QUEEN>(queen_square, them_pieces | us_pieces) & ~us_pieces;
	return pop_count(pseudo_legal_moves);
}

template<Color color>
constexpr Score evaluate_queens(Position& board) {
	Bitboard queens = board.bitboard_of(color, QUEEN);
	const Bitboard us_pieces = board.all_pieces<color>();
	const Bitboard them_pieces = board.all_pieces<~color>();

	Score score = Score();

	while (queens) {
		Square queen_square = pop_lsb(&queens);
		score += PIECE_VALUES[QUEEN];
		score += read_psqt<color>(QUEEN, queen_square);
		//Bitboard pseudo_legal_moves = attacks<QUEEN>(queen_square, them_pieces | us_pieces) & ~us_pieces;
		//Score s = MOBILITY_QUEEN[pop_count(pseudo_legal_moves)];
		//score += Score((short) (s.middle_game * 0.5), (short) (s.end_game * 0.5));
	}
	return score;
}

#endif // QUEEN_H