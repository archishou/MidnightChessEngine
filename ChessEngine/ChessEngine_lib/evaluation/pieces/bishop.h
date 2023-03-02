#ifndef BISHOP_H
#define BISHOP_H

template<Color color>
constexpr Score evaluate_bishops(Position& board) {
	Score score = Score();
	Bitboard bishops = board.bitboard_of(color, BISHOP);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	if (pop_count(bishops) >= 2) score += BISHOP_PAIR_BONUS;
	while (bishops) {
		Square bishop_square = pop_lsb(&bishops);
		score += PIECE_VALUES[BISHOP];
		score += read_psqt<color>(BISHOP, bishop_square);
		Bitboard pseudo_legal_moves = attacks<BISHOP>(bishop_square, them_pieces | us_pieces) & ~us_pieces;
		//score += MOBILITY_BISHOP[pop_count(pseudo_legal_moves)];
	}
	return score;
}

#endif // BISHOP_H