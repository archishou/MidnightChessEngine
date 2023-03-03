#include "move_generation/types.h"
#include "types.h"

//
// Created by Archishmaan Peyyety on 1/10/23.
//

// Pawn Bonuses
constexpr Score PASSED_PAWN_BONUS = S(1, 17);
constexpr Score ISOLATED_PAWN_PENALTY = S(-23, -13);
// Bishop Bonuses
constexpr Score BISHOP_PAIR_BONUS =S(32, 54);

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[NPIECE_TYPES] = {
		S(107, 80), S(412, 307), S(399, 306), S(538, 564), S(1282, 948), S(0, 0),
};

constexpr Score PAWN_TABLE[NSQUARES] = {
		S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),
		S(89, 169), S(140, 143), S(78, 128), S(124, 95), S(90, 110), S(164, 84), S(3, 148), S(-53, 188),
		S(-6, 97), S(-7, 92), S(23, 69), S(29, 39), S(68, 17), S(82, 27), S(18, 63), S(-13, 78), S(-18, 37), S(2, 20), S(3, 11), S(29, -14), S(29, -7), S(20, 1), S(13, 9), S(-19, 20), S(-32, 21), S(-23, 12), S(1, -0), S(20, -12), S(25, -8), S(18, -7), S(-1, -1), S(-24, 5), S(-21, 8), S(-25, 8), S(-2, -3), S(-2, 4), S(13, 4), S(18, -3), S(25, -11), S(-1, -6), S(-29, 23), S(-17, 8), S(-24, 19), S(-13, 19), S(-5, 19), S(37, 2), S(34, -8), S(-10, -2),
		S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),	S(0, 0),
};

constexpr Score KNIGHT_TABLE[NSQUARES] = {
		S(-210, -33), S(-115, -24), S(-48, 5), S(-50, -16), S(89, -29), S(-112, -9), S(-17, -60), S(-119, -91), S(-95, -7), S(-52, 10), S(88, -20), S(33, 14), S(27, 4), S(81, -21), S(1, -12), S(-17, -41), S(-44, -16), S(74, -11), S(52, 24), S(82, 20), S(115, -0), S(161, -6), S(102, -16), S(66, -41), S(0, -6), S(38, 14), S(30, 41), S(76, 36), S(57, 36), S(96, 21), S(40, 17), S(40, -10), S(-1, -5), S(24, 5), S(40, 28), S(32, 43), S(51, 30), S(42, 30), S(40, 17), S(7, -7), S(-11, -13), S(6, 13), S(33, 6), S(40, 24), S(52, 20), S(46, -1), S(45, -11), S(0, -20), S(-10, -32), S(-40, -7), S(6, 4), S(28, 5), S(31, 7), S(43, -13), S(-1, -14), S(3, -41), S(-132, 2), S(2, -36), S(-35, -10), S(-13, -2), S(15, -15), S(-3, -12), S(6, -39), S(-16, -67)
};

constexpr Score BISHOP_TABLE[NSQUARES] = {
		S(-33, -4), S(-7, -11), S(-145, 16), S(-90, 10), S(-64, 16), S(-56, 6), S(-30, 4), S(2, -18), S(-36, 9), S(-1, 2), S(-34, 15), S(-48, 1), S(19, 1), S(52, -5), S(5, 3), S(-57, 1), S(-20, 13), S(34, -3), S(38, -2), S(29, -2), S(29, -4), S(59, -1), S(35, 4), S(3, 12), S(-2, 6), S(11, 11), S(13, 13), S(50, 5), S(36, 7), S(28, 7), S(13, 2), S(5, 11), S(4, 2), S(20, 4), S(12, 13), S(32, 14), S(38, -1), S(8, 5), S(13, -2), S(21, -2), S(14, -4), S(29, 2), S(29, 9), S(16, 8), S(24, 12), S(45, -6), S(30, -2), S(23, -9), S(22, -5), S(40, -20), S(29, -6), S(16, 4), S(27, 0), S(37, -5), S(60, -26), S(19, -24), S(-20, -9), S(19, 2), S(16, 2), S(9, 2), S(13, 1), S(11, 5), S(-25, 8), S(-8, -8)
};

constexpr Score ROOK_TABLE[NSQUARES] = {
		S(22, 23), S(36, 16), S(-1, 32), S(55, 15), S(48, 20), S(-22, 32), S(-6, 28), S(19, 19), S(24, 22), S(17, 26), S(65, 14), S(71, 12), S(96, -6), S(86, 4), S(15, 24), S(43, 15), S(-20, 24), S(5, 22), S(7, 19), S(14, 19), S(-10, 19), S(44, 4), S(77, -3), S(8, 9), S(-33, 23), S(-29, 21), S(-10, 29), S(7, 14), S(2, 16), S(25, 12), S(-14, 12), S(-27, 22), S(-49, 27), S(-32, 25), S(-15, 24), S(-12, 20), S(3, 10), S(-13, 11), S(8, 5), S(-36, 12), S(-41, 17), S(-21, 18), S(-9, 8), S(-10, 13), S(9, 3), S(6, 2), S(0, 6), S(-27, 2), S(-31, 10), S(-8, 9), S(-6, 12), S(6, 14), S(18, 0), S(23, -0), S(10, -2), S(-53, 12), S(-1, 7), S(5, 11), S(23, 7), S(33, 2), S(36, -3), S(25, 0), S(-13, 10), S(2, -10)
};

constexpr Score QUEEN_TABLE[NSQUARES] = {
		S(-30, 1), S(-25, 45), S(-15, 45), S(-9, 41), S(115, -12), S(110, -15), S(78, -10), S(59, 32), S(-29, 1), S(-58, 30), S(-20, 38), S(-12, 51), S(-50, 84), S(61, 18), S(20, 38), S(65, 12), S(-5, -21), S(-23, 4), S(5, -16), S(-20, 57), S(23, 38), S(71, 19), S(59, 17), S(65, 22), S(-37, 25), S(-28, 19), S(-26, 17), S(-25, 26), S(-9, 47), S(-1, 47), S(-5, 78), S(0, 68), S(1, -27), S(-32, 31), S(-2, -5), S(-13, 36), S(-1, 11), S(1, 24), S(1, 54), S(3, 41), S(-12, 10), S(15, -40), S(2, 1), S(16, -30), S(11, -5), S(14, 10), S(21, 23), S(9, 43), S(-16, -12), S(12, -36), S(29, -37), S(29, -38), S(40, -48), S(44, -39), S(16, -40), S(30, -31), S(20, -34), S(22, -54), S(32, -46), S(37, -7), S(20, -13), S(-1, -21), S(-4, -9), S(-37, -29)
};

constexpr Score KING_TABLE[NSQUARES] = {
		S(-94, -78), S(183, -71), S(181, -55), S(102, -42), S(-149, 15), S(-78, 29), S(49, -3), S(43, -25), S(230, -59), S(65, 7), S(37, 9), S(117, -2), S(34, 14), S(44, 35), S(-27, 31), S(-149, 40), S(76, -6), S(76, 10), S(96, 11), S(24, 13), S(41, 12), S(120, 33), S(128, 28), S(-12, 13), S(11, -19), S(-11, 24), S(17, 26), S(-53, 40), S(-54, 38), S(-54, 46), S(-18, 33), S(-95, 16), S(-92, -9), S(16, -6), S(-74, 37), S(-132, 51), S(-139, 54), S(-96, 43), S(-86, 25), S(-109, 4), S(-8, -23), S(-25, 2), S(-61, 25), S(-98, 39), S(-96, 43), S(-85, 35), S(-36, 14), S(-54, -2), S(-3, -35), S(2, -12), S(-46, 16), S(-102, 28), S(-80, 28), S(-55, 18), S(-1, -5), S(8, -26), S(-27, -66), S(41, -51), S(11, -27), S(-96, 1), S(-14, -22), S(-49, -7), S(31, -40), S(18, -65)
};

// TODO: This kinda slow. Like 8 conditionals when it could just be PST[COLOR][TYPE][SQUARE]
template<Color color>
constexpr Score read_psqt(PieceType piece_type, Square square) {
	if (color == WHITE) square = ~square;
	switch (piece_type) {
		case PAWN: return PAWN_TABLE[square];
		case KNIGHT: return KNIGHT_TABLE[square];
		case BISHOP: return BISHOP_TABLE[square];
		case ROOK: return ROOK_TABLE[square];
		case QUEEN: return QUEEN_TABLE[square];
		case KING: return KING_TABLE[square];
		default: return {};
	}
}

constexpr Score KNIGHT_MOBILITY = S(1, 0);
constexpr Score BISHOP_MOBILITY = S(6, 4);
constexpr Score ROOK_MOBILITY = S(8, 2);
constexpr Score QUEEN_MOBILITY = S(2, 8);
