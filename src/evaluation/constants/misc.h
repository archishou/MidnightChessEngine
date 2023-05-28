#pragma once

#include "../../board/types/piece.h"
#include "../types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(96, 113),     S(450, 382),    S(457, 383),    S(631, 756),    S(1435, 1361),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(43, -0),      S(-7, 29),      S(-53, 2)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(16, 10),      S(-1, 23),      S(-20, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(7, 4),        S(1, 23),       S(4, 20),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-78, -35),    S(-72, -65),    S(75, -66),     S(-79, -11),    S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(33, 47),      S(61, 26),      S(50, 3),       S(0, 0),
		S(0, 0),        S(27, 39),      S(0, 0),        S(51, 31),      S(53, 26),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(67, 10),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(22, -11),     S(31, -12),     S(24, -6),      S(34, -10),     S(31, 8),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(203, 5),      S(121, 22),     S(75, 62),      S(124, 6),      S(66, 103),     S(0, 0)
};
constexpr Score CENTER_CONTROL[] = {
		S(0, 0),        S(-3, 3),       S(3, 10),       S(1, 1),        S(-2, 15),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(38, -18),     S(30, -11)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -11);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -24);
constexpr Score BISHOP_PAIR_BONUS = S(24, 73);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-39, -14),    S(-16, -3),     S(-4, -4),      S(1, 7),        S(9, 13),       S(14, 22),      S(22, 19),      S(28, 12),
		S(44, -10)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-36, 21),     S(-16, -18),    S(-4, -12),     S(3, -2),       S(10, 6),       S(17, 15),      S(18, 22),      S(16, 26),
		S(24, 27),      S(25, 27),      S(34, 23),      S(53, 16),      S(41, 30),      S(65, 0)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-35, -44),    S(-19, -32),    S(-15, -16),    S(-4, -26),     S(-1, -10),     S(-8, 13),      S(0, 9),        S(6, 11),
		S(11, 20),      S(17, 24),      S(17, 31),      S(26, 34),      S(32, 34),      S(34, 28),      S(73, 13)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-128, 85),    S(-37, -133),   S(-43, 20),     S(-40, -19),    S(-40, 66),     S(-35, 45),     S(-33, 58),     S(-30, 45),
		S(-26, 31),     S(-20, 17),     S(-17, 23),     S(-19, 30),     S(-13, 25),     S(-10, 21),     S(-5, 18),      S(-7, 25),
		S(-9, 21),      S(-0, 10),      S(-1, 12),      S(11, -1),      S(39, -32),     S(32, -23),     S(14, -27),     S(92, -52),
		S(21, -53),     S(60, -65),     S(165, -144),   S(355, -239)
};
constexpr Score KING_LINE_SAFETY[] = {
		S(0, 0),        S(0, 0),        S(48, -36),     S(34, 14),      S(29, 10),      S(22, 7),       S(22, 4),       S(21, 5),
		S(10, 6),       S(3, 14),       S(3, 5),        S(-3, 7),       S(-13, 6),      S(-17, 5),      S(-49, 15),     S(-44, 10),
		S(-43, 5),      S(-57, 3),      S(-71, 4),      S(-69, -2),     S(-80, -4),     S(-108, -11),   S(-122, -12),   S(-134, -32),
		S(-96, -48),    S(-22, -83),    S(0, 0),        S(0, 0)
};
constexpr Score PHALANX_PAWN[] = {
		S(0, 0),        S(2, -2),       S(19, 5),       S(30, 25),      S(51, 76),      S(68, 126),     S(-6, 405),     S(0, 0)
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
		S(0, 0),        S(-5, -7),      S(3, -5),       S(11, 10),      S(19, 18),      S(33, 58),      S(0, 0),        S(0, 0)
};
constexpr Score TEMPO = S(26, 23);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * (NPIECE_TYPES - 1) + attacked];
}