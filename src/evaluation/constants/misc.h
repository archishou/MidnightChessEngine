#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(95, 111),     S(443, 375),    S(450, 378),    S(618, 738),    S(1399, 1322),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(40, 1),       S(-7, 29),      S(-62, -2)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(14, 11),      S(-1, 23),      S(-27, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(7, 3),        S(1, 21),       S(3, 18),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-76, -33),    S(-71, -62),    S(-106, 1),     S(-79, -10),    S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(33, 46),      S(59, 25),      S(49, 4),       S(0, 0),
		S(0, 0),        S(26, 38),      S(0, 0),        S(49, 31),      S(52, 26),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(66, 8),       S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(21, -12),     S(32, -11),     S(24, -5),      S(34, -9),      S(31, 9),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(198, -1),     S(115, 22),     S(76, 62),      S(127, 8),      S(72, 99),      S(0, 0)
};
constexpr Score CENTER_CONTROL[] = {
		S(0, 0),        S(-4, 3),       S(3, 10),       S(1, 1),        S(-2, 16),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(40, -15),     S(30, -9)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-11, -10);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -24);
constexpr Score BISHOP_PAIR_BONUS = S(23, 71);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-39, -11),    S(-18, -4),     S(-6, -6),      S(-1, 5),       S(7, 10),       S(11, 19),      S(20, 17),      S(25, 11),
		S(40, -10)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-37, 19),     S(-19, -18),    S(-7, -13),     S(0, -4),       S(8, 3),        S(14, 13),      S(15, 19),      S(13, 23),
		S(21, 24),      S(21, 23),      S(30, 20),      S(47, 13),      S(36, 27),      S(57, -1)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-36, -44),    S(-20, -33),    S(-16, -18),    S(-6, -28),     S(-2, -12),     S(-10, 12),     S(-1, 7),       S(5, 9),
		S(9, 18),       S(16, 22),      S(17, 27),      S(26, 30),      S(33, 30),      S(34, 24),      S(72, 9)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-112, 28),    S(-31, -114),   S(-40, 17),     S(-38, -24),    S(-40, 68),     S(-35, 48),     S(-34, 59),     S(-30, 46),
		S(-27, 32),     S(-20, 18),     S(-17, 23),     S(-18, 28),     S(-13, 23),     S(-9, 19),      S(-4, 15),      S(-5, 22),
		S(-7, 18),      S(1, 6),        S(1, 7),        S(11, -4),      S(39, -34),     S(34, -28),     S(12, -28),     S(90, -56),
		S(17, -50),     S(67, -76),     S(123, -123),   S(327, -235)
};
constexpr Score PHALANX_PAWN[] = {
		S(0, 0),        S(3, 1),        S(18, 3),       S(26, 21),      S(50, 76),      S(39, 123),     S(44, 372),     S(0, 0)
};
constexpr Score TEMPO = S(25, 23);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * NPIECE_TYPES + attacked];
}