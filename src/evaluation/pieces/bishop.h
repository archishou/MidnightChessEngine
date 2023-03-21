#pragma once
template<Color color>
constexpr Score evaluate_bishops(Position& board) {
	Score score = Score();
	Bitboard bishops = board.bitboard_of(color, BISHOP);
	Bitboard us_pieces = board.all_pieces<color>();
	Bitboard them_pieces = board.all_pieces<~color>();
	const Bitboard all_pawns = board.bitboard_of(color, PAWN);
	if (pop_count(bishops) >= 2) score += BISHOP_PAIR_BONUS;
	while (bishops) {
		Square bishop_square = pop_lsb(&bishops);
		score += PIECE_VALUES[BISHOP];
		score += read_psqt<color>(BISHOP, bishop_square);
		Bitboard pseudo_legal_moves = attacks<BISHOP>(bishop_square, them_pieces | us_pieces) & ~us_pieces;
		score += BISHOP_MOBILITY * pop_count(pseudo_legal_moves);
		const Bitboard supporting_pawns = all_pawns & pawn_attacks<~color>(bishop_square);
		score += PAWN_PROTECTION[BISHOP] * pop_count(supporting_pawns);
	}
	return score;
}