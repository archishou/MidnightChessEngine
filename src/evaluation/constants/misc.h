#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(95, 107),     S(436, 364),    S(448, 375),    S(605, 706),    S(1388, 1311),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(44, 1),       S(-10, 31),     S(-61, -1)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(16, 9),       S(-2, 28),      S(-27, 12)
};
constexpr Score PAWN_PROTECTION[] = {
		S(4, 2),        S(1, 19),       S(2, 16),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-52, -15),    S(-60, -42),    S(-94, 23),     S(-63, 16),     S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(20, -12),     S(31, -11),     S(20, -5),      S(34, -9),      S(31, 7),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(232, -18),    S(106, -1),     S(65, 48),      S(124, -16),    S(66, 74),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(40, -13),     S(32, -9)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-16, -12);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -24);
constexpr Score BISHOP_PAIR_BONUS = S(28, 76);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-41, -0),     S(-18, -10),    S(-6, -13),     S(-2, -2),      S(6, 2),        S(10, 12),      S(19, 11),      S(24, 8),
		S(43, -13)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-38, 5),      S(-19, -31),    S(-8, -21),     S(1, -7),       S(11, 1),       S(16, 13),      S(19, 23),      S(17, 28),
		S(25, 31),      S(27, 30),      S(33, 30),      S(54, 25),      S(36, 42),      S(90, 16)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-39, -40),    S(-21, -36),    S(-17, -25),    S(-7, -33),     S(-5, -13),     S(-11, 6),      S(-3, 4),       S(2, 7),
		S(8, 14),       S(16, 19),      S(18, 22),      S(25, 29),      S(34, 28),      S(30, 25),      S(67, 13)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-111, -1),    S(-17, -78),    S(-30, 30),     S(-30, -33),    S(-32, 41),     S(-27, 5),      S(-26, 4),      S(-24, 4),
		S(-22, -7),     S(-17, -13),    S(-16, -1),     S(-17, 11),     S(-13, 11),     S(-10, 17),     S(-8, 17),      S(-10, 28),
		S(-11, 31),     S(-2, 26),      S(-7, 37),      S(3, 28),       S(36, -0),      S(19, 19),      S(-8, 33),      S(51, 15),
		S(52, -4),      S(113, -48),    S(65, -32),     S(297, -150)
};
constexpr Score TEMPO = S(0, 0);