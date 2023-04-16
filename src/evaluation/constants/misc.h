#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(101, 108),    S(448, 369),    S(462, 379),    S(630, 718),    S(1435, 1332),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(46, -1),      S(-9, 32),      S(-61, -1)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(17, 11),      S(-3, 32),      S(-27, 14)
};
constexpr Score PAWN_PROTECTION[] = {
		S(4, 2),        S(3, 22),       S(3, 19),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-81, -32),    S(-79, -58),    S(-114, 0),     S(-88, -2),     S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(34, 46),      S(61, 26),      S(49, 12),      S(0, 0),
		S(0, 0),        S(29, 40),      S(0, 0),        S(51, 32),      S(59, 36),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(76, -4),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(20, -12),     S(34, -13),     S(23, -5),      S(35, -10),     S(31, 6),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(266, 6),      S(134, 24),     S(95, 74),      S(156, 8),      S(88, 106),     S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(41, -14),     S(33, -10)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-16, -13);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -23);
constexpr Score BISHOP_PAIR_BONUS = S(25, 71);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-40, -0),     S(-16, -6),     S(-4, -11),     S(1, -0),       S(10, 3),       S(14, 14),      S(23, 12),      S(28, 9),
		S(44, -13)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-36, 5),      S(-18, -29),    S(-5, -20),     S(4, -6),       S(13, 3),       S(18, 15),      S(22, 24),      S(21, 30),
		S(30, 34),      S(32, 33),      S(36, 33),      S(58, 28),      S(52, 41),      S(92, 20)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-39, -42),    S(-21, -35),    S(-17, -23),    S(-6, -31),     S(-4, -11),     S(-10, 8),      S(-1, 6),       S(5, 9),
		S(11, 16),      S(19, 21),      S(20, 25),      S(27, 30),      S(36, 29),      S(32, 26),      S(69, 13)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-98, 15),     S(-18, -140),   S(-31, 18),     S(-32, -26),    S(-33, 39),     S(-28, 12),     S(-26, 9),      S(-23, 12),
		S(-21, 1),      S(-15, -6),     S(-13, 5),      S(-14, 17),     S(-9, 18),      S(-6, 22),      S(-4, 22),      S(-6, 33),
		S(-7, 35),      S(3, 28),       S(-2, 38),      S(6, 30),       S(40, -0),      S(23, 18),      S(0, 25),       S(56, 9),
		S(38, -5),      S(123, -57),    S(59, -42),     S(287, -152)
};
constexpr Score TEMPO = S(29, 25);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * NPIECE_TYPES + attacked];
}