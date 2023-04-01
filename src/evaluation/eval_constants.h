#pragma once
#include "move_generation/types.h"
#include "types.h"

//
// Created by Archishmaan Peyyety on 1/10/23.
//

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {S(103, 104), S(427, 360), S(422, 352), S(567, 692), S(1347, 1264), S(0, 0)};
constexpr Score PAWN_TABLE[] = {S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(205, 169), S(222, 150), S(137, 152), S(215, 95), S(162, 109), S(179, 108), S(18, 167), S(22, 192), S(-11, 109), S(12, 101), S(53, 71), S(66, 47), S(79, 35), S(94, 38), S(8, 76), S(1, 86), S(-10, 33), S(2, 19), S(9, 12), S(37, -16), S(33, -5), S(41, 3), S(13, 15), S(-7, 21), S(-18, 15), S(-14, 9), S(1, -6), S(20, -15), S(19, -5), S(20, -10), S(3, -1), S(-14, 1), S(-17, 3), S(-27, 7), S(-7, -3), S(0, -4), S(3, 9), S(-9, 8), S(-12, -1), S(-26, -2), S(-23, 22), S(-19, 12), S(-21, 23), S(-18, 17), S(-16, 29), S(-2, 22), S(-2, 4), S(-42, 4), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0)};
constexpr Score KNIGHT_TABLE[] = {S(-220, -22), S(-132, 13), S(-57, 39), S(-7, 15), S(97, 10), S(-140, 33), S(-29, -25), S(-138, -82), S(-29, 6), S(-35, 31), S(70, 24), S(34, 58), S(27, 50), S(90, 12), S(-19, 25), S(-11, -2), S(-25, 10), S(59, 26), S(69, 60), S(96, 62), S(117, 39), S(134, 42), S(75, 25), S(49, -13), S(27, 17), S(43, 45), S(58, 68), S(83, 72), S(52, 78), S(87, 69), S(40, 50), S(48, 20), S(9, 19), S(21, 34), S(47, 61), S(48, 70), S(63, 64), S(49, 63), S(68, 37), S(17, 28), S(-7, -14), S(15, 27), S(37, 25), S(41, 55), S(56, 41), S(46, 20), S(40, 4), S(-1, 14), S(-24, -19), S(-13, 8), S(16, 16), S(27, 36), S(27, 43), S(33, 9), S(8, 19), S(24, -24), S(-112, -5), S(-5, -21), S(-29, 15), S(-12, 25), S(9, 11), S(8, 14), S(-2, -6), S(-40, -33)};
constexpr Score BISHOP_TABLE[] = {S(-31, 26), S(16, 24), S(-98, 44), S(-50, 40), S(-49, 48), S(-79, 39), S(8, 26), S(7, 19), S(-32, 40), S(20, 24), S(3, 39), S(-26, 36), S(25, 34), S(49, 24), S(8, 29), S(-28, 35), S(3, 33), S(36, 25), S(56, 18), S(28, 25), S(48, 17), S(54, 31), S(57, 29), S(9, 45), S(-5, 30), S(23, 33), S(14, 36), S(69, 21), S(48, 22), S(47, 21), S(28, 40), S(12, 39), S(9, 21), S(15, 28), S(21, 33), S(48, 28), S(52, 17), S(19, 25), S(16, 18), S(41, 9), S(17, 13), S(38, 23), S(30, 24), S(26, 23), S(27, 33), S(38, 18), S(33, 14), S(37, 9), S(27, 7), S(40, 3), S(37, 11), S(17, 26), S(28, 20), S(24, 7), S(57, 22), S(17, 3), S(35, -4), S(29, 16), S(9, 36), S(12, 24), S(-6, 32), S(7, 36), S(-6, 32), S(7, 3)};
constexpr Score ROOK_TABLE[] = {S(26, 54), S(32, 50), S(-6, 61), S(10, 55), S(43, 50), S(12, 54), S(34, 48), S(64, 47), S(10, 56), S(9, 59), S(33, 52), S(44, 54), S(65, 37), S(95, 28), S(28, 50), S(53, 41), S(-14, 51), S(15, 45), S(26, 45), S(28, 43), S(49, 30), S(86, 27), S(128, 10), S(35, 27), S(-14, 44), S(-14, 45), S(10, 47), S(30, 38), S(7, 45), S(35, 39), S(6, 34), S(1, 38), S(-29, 39), S(-34, 48), S(-22, 46), S(-15, 44), S(-8, 40), S(7, 25), S(25, 27), S(-7, 23), S(-33, 24), S(-24, 34), S(-20, 27), S(-14, 29), S(-12, 29), S(5, 20), S(20, 16), S(-2, 8), S(-34, 22), S(-14, 20), S(-17, 31), S(-12, 27), S(1, 23), S(13, 15), S(14, 10), S(-58, 28), S(1, 32), S(1, 32), S(8, 31), S(18, 22), S(16, 23), S(21, 24), S(-6, 28), S(8, 12)};
constexpr Score QUEEN_TABLE[] = {S(-26, -3), S(-37, 50), S(-28, 50), S(5, 16), S(63, 25), S(68, 12), S(75, 8), S(71, 8), S(-13, 15), S(-47, 39), S(-19, 57), S(-14, 64), S(-31, 84), S(31, 64), S(14, 61), S(63, 16), S(-6, 4), S(-5, 13), S(6, 14), S(-34, 88), S(-14, 88), S(81, 45), S(83, 29), S(8, 81), S(-12, 2), S(-13, 35), S(-16, 43), S(-18, 59), S(-9, 80), S(18, 59), S(24, 81), S(8, 89), S(1, -16), S(-5, 37), S(10, 12), S(-3, 53), S(9, 34), S(20, 48), S(26, 46), S(20, 18), S(-14, 3), S(27, -50), S(21, -2), S(16, -18), S(21, -1), S(24, 5), S(37, -6), S(15, 25), S(-18, -4), S(18, -53), S(33, -54), S(24, -18), S(36, -40), S(37, -56), S(38, -72), S(4, -33), S(15, -32), S(4, -39), S(27, -42), S(32, 14), S(23, -40), S(12, -60), S(22, -63), S(-14, -68)};
constexpr Score KING_TABLE[] = {S(-104, -98), S(201, -84), S(194, -60), S(141, -59), S(-139, 2), S(-78, 24), S(99, -17), S(-0, -32), S(206, -67), S(91, 11), S(103, 9), S(120, -0), S(63, 13), S(104, 21), S(2, 36), S(-168, 25), S(41, 3), S(119, 12), S(124, 22), S(54, 16), S(92, 8), S(144, 36), S(178, 30), S(-26, 11), S(8, -15), S(47, 19), S(28, 33), S(7, 41), S(-22, 37), S(20, 31), S(12, 25), S(-114, 11), S(-85, -17), S(76, -18), S(6, 22), S(-50, 45), S(-48, 41), S(-33, 25), S(-31, 10), S(-95, -9), S(-31, -25), S(-11, -0), S(-26, 16), S(-41, 30), S(-29, 29), S(-27, 15), S(-13, 1), S(-43, -18), S(-22, -32), S(-31, -10), S(-14, 8), S(-79, 24), S(-47, 19), S(-39, 13), S(-34, 0), S(-28, -26), S(-51, -78), S(-16, -46), S(-55, -12), S(-74, -5), S(-82, -12), S(-48, -14), S(-30, -26), S(-28, -77)};
constexpr Score OPEN_FILE_BONUS[] = {S(0, 0), S(0, 0), S(0, 0), S(48, 2), S(-3, 29), S(-59, -0)};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {S(0, 0), S(0, 0), S(0, 0), S(19, 9), S(3, 26), S(-24, 11)};
constexpr Score PAWN_PROTECTION[] = {S(3, 3), S(1, 20), S(-2, 22), S(0, 0), S(0, 0), S(0, 0)};
constexpr Score ATTACKED_BY_PAWN[] = {S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0)};
constexpr Score KING_RING_ATTACK_BONUS[] = {S(16, -12), S(30, -11), S(22, -6), S(34, -9), S(32, 6), S(0, 0)};
constexpr Score CHECK_BONUS[] = {S(241, -23), S(102, -1), S(69, 47), S(123, -15), S(71, 71), S(0, 0)};
constexpr Score KING_PAWN_SHIELD[] = {S(39, -14), S(31, -8)};
constexpr Score PASSED_PAWN_BONUS = S(0, 23);
constexpr Score BLOCKED_PASSED_PAWN_PENALTY = S(4, -20);
constexpr Score ISOLATED_PAWN_PENALTY = S(-16, -14);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -18);
constexpr Score BISHOP_PAIR_BONUS = S(29, 73);
constexpr Score KNIGHT_MOBILITY = S(0, 1);
constexpr Score BISHOP_MOBILITY = S(5, 7);
constexpr Score ROOK_MOBILITY = S(4, 3);
constexpr Score QUEEN_MOBILITY = S(1, 4);

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
