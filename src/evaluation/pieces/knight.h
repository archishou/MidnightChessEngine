#pragma once
template<Color color>
constexpr Score evaluate_knight(Position& board) {
	Score score = Score();
	Bitboard knights = board.bitboard_of(color, KNIGHT);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	const Bitboard all_pawns = board.bitboard_of(color, PAWN);
	while (knights) {
		Square knight_square = pop_lsb(&knights);
		score += PIECE_VALUES[KNIGHT];
		score += read_psqt<color>(KNIGHT, knight_square);
		Bitboard pseudo_legal_moves = attacks<KNIGHT>(knight_square, them_pieces | us_pieces) & ~us_pieces;
		score += KNIGHT_MOBILITY * pop_count(pseudo_legal_moves);
		const Bitboard supporting_pawns = all_pawns & pawn_attacks<~color>(knight_square);
		score += PAWN_PROTECTION[KNIGHT] * pop_count(supporting_pawns);
	}
	return score;
}