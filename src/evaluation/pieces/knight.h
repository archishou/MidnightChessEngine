#pragma once
template<Color color>
constexpr Score evaluate_knight(Position& board) {
	Bitboard knights = board.occupancy<color, KNIGHT>();
	Bitboard us_pieces = board.occupancy<color>();
	Bitboard them_pieces = board.occupancy<~color>();

	const Bitboard all_pawns = board.occupancy<color, PAWN>();
	const Bitboard them_pawns = board.occupancy<~color, PAWN>();

	const Square them_king = lsb(board.occupancy<~color, KING>());
	const Bitboard them_king_ring = tables::attacks<KING>(them_king) & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);

	Score score = SCORE_ZERO;

	while (knights) {
		Square knight_square = pop_lsb(knights);
		score += PIECE_VALUES[KNIGHT];
		score += read_psqt<color, KNIGHT>(knight_square);

		Bitboard pseudo_legal_moves = tables::attacks<KNIGHT>(knight_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = pseudo_legal_moves & ~them_pawn_attacks;

		score += KNIGHT_MOBILITY[pop_count(mobility_squares)];

		const Bitboard supporting_pawns = all_pawns & tables::attacks<PAWN, ~color>(knight_square);
		score += PAWN_PROTECTION[KNIGHT] * pop_count(supporting_pawns);

		const Bitboard attacking_pawns = them_pawns & tables::attacks<PAWN, color>(knight_square);
		score += ATTACKED_BY_PAWN[KNIGHT] * pop_count(attacking_pawns);

		const Bitboard attacked_bishops = pseudo_legal_moves & board.occupancy<~color, BISHOP>();
		score += read_threat_bonus<KNIGHT, BISHOP>() * pop_count(attacked_bishops);

		const Bitboard attacked_rooks = pseudo_legal_moves & board.occupancy<~color, ROOK>();
		score += read_threat_bonus<KNIGHT, ROOK>() * pop_count(attacked_rooks);

		const Bitboard attacked_queens = pseudo_legal_moves & board.occupancy<~color, QUEEN>();
		score += read_threat_bonus<KNIGHT, QUEEN>() * pop_count(attacked_queens);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[KNIGHT] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & square_to_bitboard(them_king);
		score += CHECK_BONUS[KNIGHT] * king_in_check;

		score += CENTER_CONTROL[KNIGHT] * pop_count(pseudo_legal_moves & BOARD_CENTER);
	}
	return score;
}
