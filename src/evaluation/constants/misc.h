#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(99, 108),     S(448, 369),    S(460, 379),    S(621, 718),    S(1422, 1329),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(46, -1),      S(-11, 32),     S(-61, -1)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(17, 11),      S(-2, 31),      S(-27, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(4, 2),        S(1, 20),       S(2, 16),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-74, -33),    S(-81, -60),    S(-115, 1),     S(-86, -6),     S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),    S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),
		S(0, 0),        S(0, 0),    S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),
		S(0, 0),        S(0, 0),    S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),
		S(0, 0),        S(0, 0),    S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),
		S(0, 0),        S(0, 0),    S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),
		S(0, 0),        S(0, 0),    S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(20, -12),     S(30, -13),     S(19, -5),      S(34, -10),     S(31, 6),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(256, 6),      S(130, 25),     S(86, 72),      S(148, 8),      S(87, 103),     S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(41, -14),     S(32, -9)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-16, -13);
constexpr Score DOUBLED_PAWN_PENALTY = S(-16, -23);
constexpr Score BISHOP_PAIR_BONUS = S(28, 77);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-40, 1),      S(-16, -7),     S(-4, -12),     S(0, -0),       S(10, 3),       S(14, 14),      S(23, 13),      S(29, 9),
		S(48, -12)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-37, 7),      S(-18, -28),    S(-5, -19),     S(4, -5),       S(14, 4),       S(20, 16),      S(23, 25),      S(21, 30),
		S(29, 33),      S(31, 31),      S(36, 31),      S(56, 25),      S(43, 40),      S(88, 15)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-40, -40),    S(-21, -35),    S(-17, -24),    S(-6, -31),     S(-3, -11),     S(-10, 8),      S(-2, 6),       S(4, 9),
		S(10, 16),      S(18, 21),      S(20, 25),      S(25, 30),      S(35, 29),      S(31, 26),      S(69, 13)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-101, 6),     S(-17, -122),   S(-30, 20),     S(-30, -33),    S(-32, 40),     S(-27, 7),      S(-26, 7),      S(-23, 9),
		S(-21, -3),     S(-16, -7),     S(-14, 5),      S(-16, 17),     S(-11, 17),     S(-8, 23),      S(-6, 23),      S(-9, 35),
		S(-9, 36),      S(0, 30),       S(-5, 40),      S(3, 32),       S(38, 2),       S(20, 21),      S(-1, 28),      S(56, 11),
		S(44, -6),      S(126, -57),    S(66, -41),     S(287, -151)
};
constexpr Score TEMPO = S(23, 24);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * NPIECE_TYPES + attacked];
}