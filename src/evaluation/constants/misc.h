#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(94, 112),     S(444, 378),    S(451, 379),    S(620, 742),    S(1406, 1330),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(41, 1),       S(-7, 29),      S(-62, -2)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(15, 10),      S(-1, 23),      S(-27, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(7, 4),        S(1, 21),       S(4, 18),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-77, -34),    S(-71, -63),    S(-107, 1),     S(-79, -8),     S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(33, 46),      S(59, 25),      S(50, 3),       S(0, 0),
		S(0, 0),        S(27, 38),      S(0, 0),        S(50, 31),      S(52, 26),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(66, 10),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(21, -12),     S(32, -11),     S(24, -5),      S(35, -9),      S(31, 9),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(196, -0),     S(118, 21),     S(78, 62),      S(128, 7),      S(73, 98),      S(0, 0)
};
constexpr Score CENTER_CONTROL[] = {
		S(0, 0),        S(-4, 3),       S(3, 10),       S(1, 1),        S(-2, 16),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(40, -15),     S(30, -9)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-11, -10);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -24);
constexpr Score BISHOP_PAIR_BONUS = S(24, 72);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-40, -13),    S(-18, -4),     S(-5, -6),      S(-1, 5),       S(8, 11),       S(12, 20),      S(20, 17),      S(25, 11),
		S(40, -9)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-37, 19),     S(-19, -18),    S(-7, -13),     S(1, -3),       S(8, 4),        S(14, 13),      S(15, 19),      S(14, 23),
		S(21, 24),      S(21, 24),      S(30, 20),      S(49, 13),      S(35, 28),      S(57, -1)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-35, -44),    S(-19, -32),    S(-15, -18),    S(-5, -27),     S(-1, -11),     S(-9, 13),      S(-1, 8),       S(6, 10),
		S(10, 18),      S(16, 22),      S(16, 28),      S(25, 31),      S(31, 31),      S(33, 25),      S(71, 10)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-118, 56),    S(-33, -109),   S(-42, 19),     S(-39, -20),    S(-41, 69),     S(-35, 46),     S(-34, 59),     S(-30, 46),
		S(-27, 32),     S(-21, 18),     S(-17, 23),     S(-19, 29),     S(-14, 24),     S(-10, 19),     S(-6, 16),      S(-7, 23),
		S(-9, 18),      S(-0, 6),       S(-1, 7),       S(10, -4),      S(37, -35),     S(33, -28),     S(12, -29),     S(90, -57),
		S(17, -51),     S(68, -77),     S(146, -137),   S(345, -246)
};
constexpr Score PHALANX_PAWN[] = {
		S(0, 0),        S(1, -1),       S(19, 4),       S(29, 24),      S(49, 75),      S(61, 126),     S(10, 392),     S(0, 0)
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
		S(0, 0),        S(-4, -7),      S(2, -5),       S(10, 10),      S(18, 17),      S(31, 57),      S(0, 0),        S(0, 0)
};
constexpr Score TEMPO = S(25, 22);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * NPIECE_TYPES + attacked];
}