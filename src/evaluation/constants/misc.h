#pragma once

#include "../../board/types/piece.h"
#include "../types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(96, 113),     S(451, 382),    S(460, 385),    S(640, 760),    S(1445, 1376),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(41, -4),      S(-7, 26),      S(-52, 3)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(13, 11),      S(-1, 23),      S(-19, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(7, 4),        S(1, 22),       S(2, 18),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-79, -35),    S(-73, -64),    S(75, -66),     S(-80, -11),    S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(34, 46),      S(61, 26),      S(50, 4),       S(0, 0),
		S(0, 0),        S(27, 39),      S(0, 0),        S(51, 32),      S(54, 24),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(67, 11),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(22, -11),     S(29, -12),     S(23, -6),      S(34, -10),     S(31, 7),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(205, 5),      S(122, 22),     S(75, 62),      S(123, 7),      S(66, 104),     S(0, 0)
};
constexpr Score CENTER_CONTROL[] = {
		S(0, 0),        S(-4, 3),       S(3, 11),       S(0, 1),        S(-2, 15),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(38, -18),     S(31, -11)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -11);
constexpr Score DOUBLED_PAWN_PENALTY = S(-17, -24);
constexpr Score BISHOP_PAIR_BONUS = S(25, 73);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-13, 21),     S(4, 19),       S(10, 8),       S(10, 13),      S(12, 14),      S(11, 19),      S(14, 14),      S(14, 6),
		S(26, -16)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-26, 33),     S(-8, -14),     S(1, -8),       S(6, -2),       S(11, 7),       S(16, 14),      S(16, 21),      S(13, 24),
		S(19, 26),      S(18, 24),      S(28, 20),      S(46, 12),      S(33, 28),      S(56, -5)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-27, -46),    S(-14, -30),    S(-12, -13),    S(-2, -24),     S(1, -8),       S(-5, 14),      S(1, 12),       S(8, 12),
		S(9, 21),       S(14, 24),      S(14, 29),      S(21, 32),      S(26, 32),      S(26, 27),      S(61, 11)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-121, 102),   S(-32, -111),   S(-47, 46),     S(-39, -2),     S(-43, 86),     S(-36, 56),     S(-36, 71),     S(-31, 54),
		S(-29, 42),     S(-22, 22),     S(-19, 28),     S(-21, 30),     S(-15, 24),     S(-11, 17),     S(-5, 11),      S(-7, 19),
		S(-8, 12),      S(3, -2),       S(0, -1),       S(13, -15),     S(41, -49),     S(36, -39),     S(16, -43),     S(101, -78),
		S(23, -71),     S(68, -88),     S(176, -173),   S(350, -256)
};
constexpr Score KNIGHT_FORWARD_MOBILITY[] = {
		S(-19, -28),    S(-12, -5),     S(-3, 3),       S(7, 7),        S(22, 6),       S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0)
};
constexpr Score BISHOP_FORWARD_MOBILITY[] = {
		S(-5, -11),     S(-4, 2),       S(-1, -1),      S(2, 3),        S(6, 2),        S(3, 2),        S(6, -1),       S(11, 9),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ROOK_FORWARD_MOBILITY[] = {
		S(-5, -2),      S(-2, -9),      S(-1, -5),      S(4, -1),       S(5, 5),        S(3, 7),        S(5, 13),       S(5, 26),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score QUEEN_FORWARD_MOBILITY[] = {
		S(-3, -37),     S(5, -20),      S(2, -8),       S(6, -26),      S(4, -5),       S(4, -7),       S(3, 0),        S(5, 1),
		S(2, 1),        S(-2, 13),      S(5, 18),       S(-7, 33),      S(-27, 59),     S(-15, 98),     S(-30, 80),     S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_LINE_SAFETY[] = {
		S(0, 0),        S(0, 0),        S(50, -37),     S(36, 13),      S(30, 10),      S(23, 6),       S(23, 3),       S(21, 5),
		S(10, 6),       S(3, 14),       S(2, 6),        S(-4, 8),       S(-14, 6),      S(-18, 6),      S(-51, 16),     S(-46, 10),
		S(-46, 6),      S(-60, 3),      S(-73, 5),      S(-72, -1),     S(-82, -3),     S(-111, -10),   S(-125, -11),   S(-139, -31),
		S(-102, -47),   S(-25, -83),    S(0, 0),        S(0, 0)
};
constexpr Score PHALANX_PAWN[] = {
		S(0, 0),        S(2, -2),       S(18, 5),       S(28, 25),      S(50, 76),      S(65, 126),     S(-5, 404),     S(0, 0)
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
		S(0, 0),        S(-5, -7),      S(3, -5),       S(11, 10),      S(19, 18),      S(31, 58),      S(0, 0),        S(0, 0)
};
constexpr Score TEMPO = S(26, 23);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * (NPIECE_TYPES - 1) + attacked];
}