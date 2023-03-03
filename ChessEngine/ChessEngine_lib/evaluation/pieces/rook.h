#ifndef ROOK_H
#define ROOK_H

template<Color color>
constexpr Score evaluate_rooks(Position& board) {
	Score score = Score();
	Bitboard rooks = board.bitboard_of(color, ROOK);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	while (rooks) {
		Square rook_square = pop_lsb(&rooks);
		score += PIECE_VALUES[ROOK];
		score += read_psqt<color>(ROOK, rook_square);
		Bitboard pseudo_legal_moves = attacks<ROOK>(rook_square, them_pieces | us_pieces) & ~us_pieces;
		score += ROOK_MOBILITY * pop_count(pseudo_legal_moves);
	}
	return score;
}

#endif // ROOK_H