#pragma once

#include "../../board/types/piece.h"
#include "../types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(95, 113),     S(447, 380),    S(453, 381),    S(624, 748),    S(1420, 1340),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(41, 1),       S(-7, 29),      S(-63, -2)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(15, 11),      S(-1, 23),      S(-27, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(7, 4),        S(1, 22),       S(4, 19),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-77, -34),    S(-71, -64),    S(73, -65),     S(-79, -9),     S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(33, 46),      S(60, 25),      S(50, 3),       S(0, 0),
		S(0, 0),        S(27, 38),      S(0, 0),        S(50, 31),      S(52, 26),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(67, 10),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(21, -13),     S(33, -11),     S(24, -5),      S(35, -9),      S(31, 9),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(198, -1),     S(118, 21),     S(79, 61),      S(129, 7),      S(74, 98),      S(0, 0)
};
constexpr Score CENTER_CONTROL[] = {
		S(0, 0),        S(-4, 3),       S(3, 11),       S(1, 1),        S(-2, 16),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(40, -15),     S(30, -10)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -10);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -24);
constexpr Score BISHOP_PAIR_BONUS = S(24, 72);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-39, -13),    S(-17, -4),     S(-5, -6),      S(0, 6),        S(8, 11),       S(12, 20),      S(21, 18),      S(26, 12),
		S(42, -9)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-36, 21),     S(-17, -17),    S(-5, -12),     S(2, -2),       S(9, 5),        S(15, 14),      S(17, 20),      S(15, 24),
		S(22, 25),      S(23, 25),      S(32, 21),      S(50, 14),      S(36, 29),      S(59, 0)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-35, -44),    S(-19, -32),    S(-15, -17),    S(-4, -26),     S(-1, -10),     S(-9, 14),      S(-1, 9),       S(6, 10),
		S(10, 19),      S(16, 23),      S(16, 29),      S(26, 32),      S(32, 32),      S(33, 26),      S(72, 11)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-123, 74),    S(-33, -123),   S(-43, 22),     S(-40, -17),    S(-41, 71),     S(-36, 47),     S(-34, 59),     S(-30, 47),
		S(-27, 32),     S(-21, 19),     S(-17, 23),     S(-19, 29),     S(-14, 24),     S(-10, 19),     S(-6, 16),      S(-7, 23),
		S(-9, 18),      S(-0, 7),       S(-1, 8),       S(11, -4),      S(38, -35),     S(33, -28),     S(13, -29),     S(90, -57),
		S(17, -51),     S(71, -78),     S(151, -139),   S(353, -250)
};
constexpr Score PHALANX_PAWN[] = {
		S(0, 0),        S(2, -1),       S(19, 4),       S(29, 25),      S(49, 76),      S(62, 128),     S(-9, 400),     S(0, 0)
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
		S(0, 0),        S(-4, -7),      S(3, -5),       S(10, 10),      S(19, 18),      S(31, 58),      S(0, 0),        S(0, 0)
};
constexpr Score TEMPO = S(25, 22);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * (NPIECE_TYPES - 1) + attacked];
}