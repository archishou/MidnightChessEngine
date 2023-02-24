#include "move_generation/position.h"
#include "piece_square_tables.h"

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 340;
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

static int manhattan_dist(Square s1, Square s2) {
	int rank_dist = abs(rank_of(s1) - rank_of(s2));
	int file_dist = abs(file_of(s1) - file_of(s2));
	return rank_dist + file_dist;
}

static double endgame_weight(int material_wo_pawns) {
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
int evaluate_all_piece_positions(Position& board) {
	int mg_score_us = 0, eg_score_us = 0;
	int mg_score_them = 0, eg_score_them = 0;
	int game_phase = 0;
	for (PieceType piece_type : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
		Bitboard piece_bitboard_us = board.bitboard_of(color, piece_type);
		Bitboard piece_bitboard_them = board.bitboard_of(~color, piece_type);
		while (piece_bitboard_us) {
			Square square = pop_lsb(&piece_bitboard_us);
			mg_score_us += read_pstq<color>(MIDDLE, piece_type, square) + PIECE_VALUES[MIDDLE][piece_type];
			eg_score_us += read_pstq<color>(END, piece_type, square) + PIECE_VALUES[END][piece_type];
			game_phase += GAME_PHASE_BONUS[piece_type];
		}
		while (piece_bitboard_them) {
			Square square = pop_lsb(&piece_bitboard_them);
			mg_score_them += read_pstq<~color>(MIDDLE, piece_type, square) + PIECE_VALUES[MIDDLE][piece_type];
			eg_score_them += read_pstq<~color>(END, piece_type, square) + PIECE_VALUES[END][piece_type];
			game_phase += GAME_PHASE_BONUS[piece_type];
		}
	}
	int mg_score = mg_score_us - mg_score_them;
	int eg_score = eg_score_us - eg_score_them;
	int mg_phase = std::min(game_phase, 24);
	int eg_phase = 24 - mg_phase;
	return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}

template<Color color>
int evaluate(Position& board) {

	int us_material = evaluate_material<color>(board);
	int their_material = evaluate_material<~color>(board);

	int us_material_wo_pawns = us_material - pop_count(board.bitboard_of(color, PAWN)) * PAWN_VALUE;
	int their_material_wo_pawns = their_material - pop_count(board.bitboard_of(~color, PAWN)) * PAWN_VALUE;

	double us_end_weight = endgame_weight(us_material_wo_pawns);
	double their_end_weight = endgame_weight(their_material_wo_pawns);

	/*
	int us_piece_positions = evaluate_all_piece_positions<color>(board);
	int their_piece_positions = evaluate_all_piece_positions<~color>(board);
	 */

	int us_mop_up_eval = mop_up_eval<color>(board, us_material, their_material, us_end_weight);
	int their_mop_up_eval = mop_up_eval<~color>(board, their_material, us_material, their_end_weight);
	 /*

	int us_eval = us_material + us_piece_positions + us_mop_up_eval;
	int their_eval = their_material + their_piece_positions + their_mop_up_eval;
	return us_eval - their_eval;
	 */
	int piece_position = evaluate_all_piece_positions<color>(board);
	int mop_up_eval = us_mop_up_eval - their_mop_up_eval;
	return piece_position + mop_up_eval;
}