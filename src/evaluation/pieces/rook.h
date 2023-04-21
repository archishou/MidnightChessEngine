#pragma once
template<Color color>
constexpr Score evaluate_rooks(Position& board) {
	Score score = SCORE_ZERO;
	Bitboard rooks = board.bitboard_of(color, ROOK);

	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	const Bitboard them_pawns = board.bitboard_of(~color, PAWN);
	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);
	const Square them_king = bsf(board.bitboard_of(~color, KING));
	const Bitboard them_king_ring = KING_ATTACKS[them_king] & ~them_pieces;

	const Bitboard them_pawn_attacks = pawn_attacks<~color>(them_pawns);
	const Bitboard xray_occupancy = us_pieces ^ rooks ^ board.bitboard_of(color, QUEEN);
	while (rooks) {
		Square rook_square = pop_lsb(&rooks);
		const Bitboard rook_square_bb = SQUARE_BB[rook_square];
		score += PIECE_VALUES[ROOK];
		score += read_psqt<color, ROOK>(rook_square);

		Bitboard pseudo_legal_moves = attacks<ROOK>(rook_square, them_pieces | us_pieces) & ~us_pieces;
		Bitboard mobility_squares = attacks<ROOK>(rook_square, them_pieces | xray_occupancy) & ~(xray_occupancy | them_pawn_attacks);

		score += ROOK_MOBILITY[pop_count(mobility_squares)];

		const bool on_open_file = rook_square_bb & board_open_files;
		const bool on_semi_open_file = rook_square_bb & board_semi_open_files;
		score += (OPEN_FILE_BONUS[ROOK] * on_open_file) + (SEMI_OPEN_FILE_BONUS[ROOK] * on_semi_open_file);

		const Bitboard attacking_pawns = them_pawns & pawn_attacks<color>(rook_square);
		score += ATTACKED_BY_PAWN[ROOK] * pop_count(attacking_pawns);

		const Bitboard attacked_queens = pseudo_legal_moves & board.bitboard_of(~color, QUEEN);
		score += read_threat_bonus<ROOK, QUEEN>() * pop_count(attacked_queens);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[ROOK] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & SQUARE_BB[them_king];
		score += CHECK_BONUS[ROOK] * king_in_check;

		score += CENTER_CONTROL[ROOK] * pop_count(pseudo_legal_moves & BOARD_CENTER);
	}
	return score;
}
