#pragma once
template<Color color>
constexpr Score evaluate_knight(Position& board) {
	Bitboard knights = board.bitboard_of(color, KNIGHT);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();

	const Bitboard all_pawns = board.bitboard_of(color, PAWN);
	const Bitboard them_pawns = board.bitboard_of(~color, PAWN);

	const Square them_king = bsf(board.bitboard_of(~color, KING));
	const Bitboard them_king_ring = KING_ATTACKS[them_king] & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);

	Score score = SCORE_ZERO;

	while (knights) {
		Square knight_square = pop_lsb(&knights);
		score += PIECE_VALUES[KNIGHT];
		score += read_psqt<color, KNIGHT>(knight_square);

		Bitboard pseudo_legal_moves = attacks<KNIGHT>(knight_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = pseudo_legal_moves & ~them_pawn_attacks;

		score += KNIGHT_MOBILITY[pop_count(mobility_squares)];

		const Bitboard supporting_pawns = all_pawns & pawn_attacks<~color>(knight_square);
		score += PAWN_PROTECTION[KNIGHT] * pop_count(supporting_pawns);

		const Bitboard attacking_pawns = them_pawns & pawn_attacks<color>(knight_square);
		score += ATTACKED_BY_PAWN[KNIGHT] * pop_count(attacking_pawns);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[KNIGHT] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & SQUARE_BB[them_king];
		score += CHECK_BONUS[KNIGHT] * king_in_check;
	}
	return score;
}