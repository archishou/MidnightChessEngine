#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"

// If this is updated be sure compute_game_phase loop is also updated. Currently only checks non-zeros.
constexpr int GAME_PHASE_BONUS[NPIECE_TYPES] = {
		0, 1, 1, 2, 4, 0
};

constexpr Score PIECE_VALUES[] = {
		S(98, 113),     S(441, 374),    S(448, 377),    S(613, 734),    S(1386, 1313),  S(0, 0)
};
constexpr Score OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(39, 1),       S(-7, 29),      S(-61, -2)
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(14, 11),      S(-1, 24),      S(-26, 13)
};
constexpr Score PAWN_PROTECTION[] = {
		S(4, 2),        S(1, 21),       S(3, 19),       S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score ATTACKED_BY_PAWN[] = {
		S(0, 0),        S(-75, -35),    S(-70, -63),    S(-105, -0),    S(-79, -8),     S(0, 0)
};
constexpr Score THREATS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(33, 45),      S(59, 25),      S(49, 4),       S(0, 0),
		S(0, 0),        S(26, 38),      S(0, 0),        S(49, 30),      S(52, 26),      S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(66, 8),       S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
		S(21, -12),     S(33, -11),     S(24, -5),      S(34, -9),      S(31, 9),       S(0, 0)
};
constexpr Score CHECK_BONUS[] = {
		S(189, 2),      S(115, 22),     S(75, 62),      S(126, 8),      S(72, 98),      S(0, 0)
};
constexpr Score CENTER_CONTROL[] = {
		S(0, 0),        S(-4, 3),       S(3, 10),       S(1, 1),        S(-2, 16),      S(0, 0)
};
constexpr Score KING_PAWN_SHIELD[] = {
		S(41, -15),     S(32, -8)
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-15, -13);
constexpr Score DOUBLED_PAWN_PENALTY = S(-17, -22);
constexpr Score BISHOP_PAIR_BONUS = S(23, 71);
constexpr Score KNIGHT_MOBILITY[] = {
		S(-41, -12),    S(-19, -4),     S(-7, -7),      S(-2, 5),       S(6, 10),       S(10, 19),      S(19, 16),      S(25, 10),
		S(40, -10)
};
constexpr Score BISHOP_MOBILITY[] = {
		S(-36, 17),     S(-19, -20),    S(-7, -14),     S(-1, -4),      S(6, 3),        S(12, 12),      S(13, 18),      S(12, 22),
		S(19, 23),      S(19, 23),      S(27, 19),      S(45, 13),      S(34, 26),      S(56, -2)
};
constexpr Score ROOK_MOBILITY[] = {
		S(-36, -45),    S(-20, -34),    S(-16, -19),    S(-6, -28),     S(-3, -12),     S(-10, 12),     S(-2, 7),       S(4, 9),
		S(8, 18),       S(16, 21),      S(16, 27),      S(26, 30),      S(32, 30),      S(33, 23),      S(71, 9)
};
constexpr Score QUEEN_MOBILITY[] = {
		S(-111, 15),    S(-26, -136),   S(-38, 14),     S(-37, -23),    S(-38, 69),     S(-34, 46),     S(-33, 58),     S(-29, 44),
		S(-25, 31),     S(-19, 17),     S(-15, 22),     S(-17, 27),     S(-11, 23),     S(-7, 19),      S(-3, 15),      S(-4, 22),
		S(-6, 17),      S(2, 7),        S(1, 8),        S(12, -3),      S(39, -34),     S(34, -28),     S(11, -26),     S(90, -55),
		S(17, -48),     S(66, -74),     S(94, -104),    S(306, -221)
};
constexpr Score TEMPO = S(25, 23);

template<PieceType attacking, PieceType attacked>
Score read_threat_bonus() {
	return THREATS[attacking * NPIECE_TYPES + attacked];
}