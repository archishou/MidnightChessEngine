#include "move_generation/position.h"
#include "piece_square_tables.h"

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
const int KING_VALUE = 1000;

const int ENDGAME_MATERIAL_THRESHOLD = ROOK_VALUE * 2 + BISHOP_VALUE + KNIGHT_VALUE;

const int center_dist[64] = {
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 2, 2, 2, 2, 2, 2, 3,
	3, 2, 1, 1, 1, 1, 2, 3,
	3, 2, 1, 0, 0, 1, 2, 3,
	3, 2, 1, 0, 0, 1, 2, 3,
	3, 2, 1, 1, 1, 1, 2, 3,
	3, 2, 2, 2, 2, 2, 2, 3,
	3, 3, 3, 3, 3, 3, 3, 3
};

int manhattan_dist(Square s1, Square s2) {
	int rank_dist = abs(rank_of(s1) - rank_of(s2));
	int file_dist = abs(file_of(s1) - file_of(s2));
	return rank_dist + file_dist;
}

double endgame_weight(int material_wo_pawns) {
	const double multiplier = 1.0 / ENDGAME_MATERIAL_THRESHOLD;
	return 1 - std::min(1.0, material_wo_pawns * multiplier);
}

template<Color color>
int mop_up_eval(Position& board, int us_material, int their_material, float end_weight) {
	if (us_material > their_material + PAWN_VALUE * 2 && end_weight > 0) {
		Square us_king = bsf(board.bitboard_of(color, KING));
		Square their_king = bsf(board.bitboard_of(~color, KING));
		int score = center_dist[their_king];
		score += (14 - manhattan_dist(us_king, their_king)) * 4;
		return score;
	}
	return 0;
}

template<Color color>
int evaluate_material(Position& board) {
	// TODO: better ways to handle mate, this is not ideal and can result in mis-evaluations of a position.
	int usPawns = pop_count(board.bitboard_of(color, PAWN));
	int usKnight = pop_count(board.bitboard_of(color, KNIGHT));
	int usBishop = pop_count(board.bitboard_of(color, BISHOP));
	int usRook = pop_count(board.bitboard_of(color, ROOK));
	int usQueen = pop_count(board.bitboard_of(color, QUEEN));

	return (PAWN_VALUE * usPawns) + (KNIGHT_VALUE * usKnight) + (BISHOP_VALUE * usBishop) +
		   (ROOK_VALUE * usRook) + (QUEEN_VALUE * usQueen);
}

template<Color color>
int evaluate_piece_position(Position& board, PieceType piece_type) {
	int pstq_score = 0;
	Bitboard piece_bitboard = board.bitboard_of(color, piece_type);
	while (piece_bitboard != 0) {
		Square square = pop_lsb(&piece_bitboard);
		pstq_score += read_pstq<color>(piece_type, square);
	}
	return pstq_score;
}

template<Color color>
int evaluate_all_piece_positions(Position& board, double endgame_weight) {
	int pstq_score = 0;
	for (PieceType piece_type : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN}) {
		pstq_score += evaluate_piece_position<color>(board, piece_type);
	}
	pstq_score += evaluate_piece_position<color>(board, KING) * (1 - endgame_weight);
	return pstq_score;
}

template<Color color>
int evaluate_all_piece_positions(Position& board) {
	evaluate_all_piece_positions<color>(board, 1);
}

template<Color color>
int evaluate(Position& board) {
	int us_material = evaluate_material<color>(board);
	int their_material = evaluate_material<~color>(board);

	int us_material_wo_pawns = us_material - pop_count(board.bitboard_of(color, PAWN)) * PAWN_VALUE;
	int their_material_wo_pawns = their_material - pop_count(board.bitboard_of(~color, PAWN)) * PAWN_VALUE;

	double us_end_weight = endgame_weight(us_material_wo_pawns);
	double their_end_weight = endgame_weight(their_material_wo_pawns);

	int us_piece_positions = evaluate_all_piece_positions<color>(board, us_end_weight);
	int their_piece_positions = evaluate_all_piece_positions<~color>(board, their_end_weight);

	int us_mop_up_eval = mop_up_eval<color>(board, us_material, their_material, us_end_weight);
	int their_mop_up_eval = mop_up_eval<~color>(board, their_material, us_material, their_end_weight);

	int us_eval = us_material + us_piece_positions + us_mop_up_eval;
	int their_eval = their_material + their_piece_positions + their_mop_up_eval;
	return us_eval - their_eval;
}