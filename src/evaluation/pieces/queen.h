#pragma once
template<Color color>
constexpr Score evaluate_queens(Position& board) {
	Bitboard queens = board.bitboard_of(color, QUEEN);
	const Bitboard us_pieces = board.all_pieces<color>();
	const Bitboard them_pieces = board.all_pieces<~color>();

	const Bitboard board_open_files = open_files(board);
	const Bitboard board_semi_open_files = semi_open_files<color>(board);

	const Square them_king = bsf(board.bitboard_of(~color, KING));
	const Bitboard them_king_ring = KING_ATTACKS[them_king] & ~them_pieces;

	Score score = Score();

	while (queens) {
		Square queen_square = pop_lsb(&queens);
		const Bitboard queen_square_bb = SQUARE_BB[queen_square];
		score += PIECE_VALUES[QUEEN];
		score += read_psqt<color>(QUEEN, queen_square);

		Bitboard pseudo_legal_moves = attacks<QUEEN>(queen_square, them_pieces | us_pieces) & ~us_pieces;
		score += QUEEN_MOBILITY * pop_count(pseudo_legal_moves);

		const bool on_open_file = queen_square_bb & board_open_files;
		const bool on_semi_open_file = queen_square_bb & board_semi_open_files;
		score += (OPEN_FILE_BONUS[QUEEN] * on_open_file) + (SEMI_OPEN_FILE_BONUS[QUEEN] * on_semi_open_file);

		const Bitboard king_ring_attacks = pseudo_legal_moves & them_king_ring;
		score += KING_RING_ATTACK_BONUS[QUEEN] * pop_count(king_ring_attacks);

		const bool king_in_check = pseudo_legal_moves & SQUARE_BB[them_king];
		score += CHECK_BONUS[QUEEN] * king_in_check;
	}
	return score;
}