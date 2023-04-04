#pragma once
template<Color color>
constexpr Score evaluate_bishops(Position& board) {
	Score score = SCORE_ZERO;

	Bitboard bishops = board.bitboard_of(color, BISHOP);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	const Bitboard all_pawns = board.bitboard_of(color, PAWN);
	const Bitboard them_pawns = board.bitboard_of(~color, PAWN);
	const Square them_king = bsf(board.bitboard_of(~color, KING));
	const Bitboard them_king_ring = KING_ATTACKS[them_king] & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);
	const Bitboard xray_occupancy = us_pieces ^ bishops ^ board.bitboard_of(color, QUEEN);

	const bool bishop_pair = pop_count(bishops) >> 1;
	score += BISHOP_PAIR_BONUS * bishop_pair;

	while (bishops) {
		Square bishop_square = pop_lsb(&bishops);
		score += PIECE_VALUES[BISHOP];
		score += read_psqt<color, BISHOP>(bishop_square);

		Bitboard pseudo_legal_moves = attacks<BISHOP>(bishop_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = attacks<BISHOP>(bishop_square, them_pieces | xray_occupancy) & ~(xray_occupancy | them_pawn_attacks);

		score += BISHOP_MOBILITY[pop_count(mobility_squares)];

		const Bitboard supporting_pawns = all_pawns & pawn_attacks<~color>(bishop_square);
		score += PAWN_PROTECTION[BISHOP] * pop_count(supporting_pawns);

		const Bitboard attacking_pawns = them_pawns & pawn_attacks<color>(bishop_square);
		score += ATTACKED_BY_PAWN[BISHOP] * pop_count(attacking_pawns);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[BISHOP] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & SQUARE_BB[them_king];
		score += CHECK_BONUS[BISHOP] * king_in_check;
	}
	return score;
}