#pragma once
#include "evaluation/types.h"
#include "board/types/board_types.h"
#include "board/types/square.h"

constexpr Score PAWN_TABLE[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(114, 156),    S(86, 161),     S(62, 153),     S(92, 128),     S(53, 138),     S(89, 145),     S(13, 177),     S(33, 177),
		S(2, 44),       S(7, 38),       S(35, 37),      S(56, 5),       S(68, 16),      S(85, 22),      S(36, 36),      S(17, 46),
		S(-5, 18),      S(-3, 8),       S(-0, 9),       S(22, -17),     S(20, -1),      S(41, 2),       S(2, 11),       S(-5, 21),
		S(-14, 8),      S(-18, 6),      S(-2, -6),      S(8, -11),      S(11, -3),      S(22, -8),      S(2, -4),       S(-7, -2),
		S(-20, 3),      S(-21, 8),      S(-7, 10),      S(-3, 7),       S(5, 23),       S(-8, 22),      S(-9, 8),       S(-26, 3),
		S(-11, 15),     S(-3, 15),      S(-7, 31),      S(1, 14),       S(5, 39),       S(12, 36),      S(14, 15),      S(-33, 3),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score KNIGHT_TABLE[] = {
		S(-215, -17),   S(-132, 13),    S(-90, 46),     S(-9, 14),      S(95, 6),       S(-152, 39),    S(-42, -22),    S(-137, -61),
		S(-25, 14),     S(-38, 34),     S(34, 22),      S(21, 54),      S(31, 38),      S(59, 7),       S(-16, 23),     S(3, 2),
		S(-38, 18),     S(24, 29),      S(46, 68),      S(79, 63),      S(86, 47),      S(116, 48),     S(58, 19),      S(57, -8),
		S(26, 24),      S(45, 45),      S(57, 67),      S(89, 70),      S(61, 73),      S(82, 71),      S(42, 51),      S(58, 30),
		S(11, 31),      S(28, 28),      S(49, 65),      S(68, 69),      S(77, 62),      S(61, 65),      S(85, 37),      S(28, 36),
		S(-5, -22),     S(17, 17),      S(35, 31),      S(48, 56),      S(59, 44),      S(47, 23),      S(43, 5),       S(12, 25),
		S(-23, -7),     S(-11, 18),     S(15, 4),       S(36, 32),      S(34, 42),      S(35, 2),       S(15, 17),      S(22, -7),
		S(-115, 20),    S(2, -2),       S(-13, 17),     S(6, 29),       S(21, 16),      S(15, 23),      S(3, 18),       S(-37, -21)
};
constexpr Score BISHOP_TABLE[] = {
		S(-41, 16),     S(-2, 27),      S(-100, 55),    S(-71, 61),     S(-60, 63),     S(-85, 44),     S(-11, 32),     S(31, 3),
		S(-20, 41),     S(7, 29),       S(6, 42),       S(-18, 37),     S(-0, 42),      S(40, 33),      S(-15, 33),     S(-21, 31),
		S(22, 39),      S(35, 36),      S(64, 37),      S(30, 39),      S(55, 30),      S(65, 53),      S(68, 38),      S(30, 48),
		S(1, 52),       S(29, 41),      S(21, 49),      S(68, 30),      S(46, 32),      S(47, 39),      S(25, 48),      S(22, 61),
		S(11, 42),      S(20, 30),      S(33, 45),      S(44, 34),      S(53, 24),      S(22, 43),      S(24, 23),      S(37, 33),
		S(17, 24),      S(33, 28),      S(38, 39),      S(32, 40),      S(31, 49),      S(42, 33),      S(40, 18),      S(40, 16),
		S(37, 9),       S(42, 2),       S(43, 18),      S(29, 27),      S(34, 26),      S(32, 11),      S(60, 24),      S(38, -7),
		S(46, -13),     S(34, 18),      S(21, 59),      S(23, 44),      S(15, 37),      S(14, 47),      S(14, 30),      S(26, -13)
};
constexpr Score ROOK_TABLE[] = {
		S(34, 59),      S(31, 63),      S(-9, 66),      S(-3, 67),      S(47, 51),      S(45, 48),      S(33, 55),      S(99, 46),
		S(21, 62),      S(13, 66),      S(38, 57),      S(54, 54),      S(56, 48),      S(94, 27),      S(46, 48),      S(61, 44),
		S(-10, 58),     S(27, 48),      S(38, 44),      S(50, 40),      S(61, 32),      S(106, 31),     S(139, 14),     S(65, 25),
		S(-7, 54),      S(5, 48),       S(27, 51),      S(40, 41),      S(33, 45),      S(51, 38),      S(29, 39),      S(12, 46),
		S(-27, 51),     S(-18, 53),     S(-9, 47),      S(4, 45),       S(3, 43),       S(16, 32),      S(27, 31),      S(5, 29),
		S(-33, 29),     S(-14, 33),     S(-13, 30),     S(-7, 29),      S(-1, 30),      S(17, 16),      S(31, 21),      S(1, 12),
		S(-42, 32),     S(-10, 24),     S(-20, 32),     S(-8, 27),      S(-2, 26),      S(11, 15),      S(12, 12),      S(-66, 33),
		S(-15, 38),     S(-14, 35),     S(-9, 31),      S(2, 19),       S(1, 19),       S(6, 25),       S(-3, 25),      S(-1, 13)
};
constexpr Score QUEEN_TABLE[] = {
		S(-19, 16),     S(-35, 85),     S(1, 42),       S(36, 16),      S(77, 30),      S(89, 25),      S(86, 41),      S(52, 44),
		S(-7, 51),      S(-41, 80),     S(-13, 55),     S(-20, 66),     S(-22, 83),     S(55, 57),      S(18, 93),      S(47, 75),
		S(-7, 13),      S(-4, 20),      S(17, 3),       S(-6, 49),      S(1, 59),       S(91, 24),      S(107, 14),     S(15, 85),
		S(-6, 21),      S(-3, 32),      S(-13, 28),     S(-5, 13),      S(9, 28),       S(29, 44),      S(40, 66),      S(19, 100),
		S(-2, 20),      S(6, 29),       S(9, 1),        S(6, 12),       S(16, -3),      S(28, 22),      S(43, 19),      S(26, 31),
		S(-9, -9),      S(27, -58),     S(19, -13),     S(11, -22),     S(19, -18),     S(24, -6),      S(46, -28),     S(18, -1),
		S(-15, 1),      S(15, -30),     S(28, -74),     S(18, -20),     S(22, -27),     S(37, -82),     S(29, -56),     S(-11, -5),
		S(9, -25),      S(-17, -4),     S(-1, -19),     S(4, 51),       S(-2, -10),     S(-8, -43),     S(16, -36),     S(-13, -53)
};
constexpr Score KING_TABLE[] = {
		S(-82, -108),   S(209, -86),    S(188, -62),    S(102, -55),    S(-184, 0),     S(-95, 22),     S(70, -18),     S(-26, -41),
		S(170, -64),    S(138, 7),      S(102, 11),     S(122, -0),     S(77, 11),      S(77, 27),      S(-3, 41),      S(-154, 15),
		S(43, 7),       S(110, 23),     S(117, 26),     S(29, 26),      S(100, 14),     S(126, 43),     S(177, 31),     S(-42, 11),
		S(-26, 1),      S(13, 34),      S(39, 38),      S(26, 45),      S(-14, 45),     S(36, 36),      S(23, 31),      S(-105, 12),
		S(-74, -17),    S(83, -13),     S(17, 27),      S(-32, 49),     S(-10, 45),     S(-16, 28),     S(-10, 11),     S(-82, -10),
		S(-56, -20),    S(-12, 1),      S(-21, 19),     S(-26, 34),     S(-14, 32),     S(-14, 16),     S(-12, 1),      S(-34, -22),
		S(-30, -23),    S(-44, -2),     S(-15, 7),      S(-76, 24),     S(-40, 18),     S(-41, 14),     S(-41, 2),      S(-29, -30),
		S(-57, -85),    S(-12, -48),    S(-58, -10),    S(-70, -10),    S(-81, -15),    S(-45, -19),    S(-21, -36),    S(-27, -87)
};
constexpr Score PASSED_PAWN_BONUS[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(114, 156),    S(86, 161),     S(62, 153),     S(92, 128),     S(53, 138),     S(89, 145),     S(13, 177),     S(33, 177),
		S(76, 191),     S(50, 192),     S(51, 156),     S(21, 156),     S(8, 134),      S(12, 151),     S(-26, 180),    S(18, 177),
		S(33, 111),     S(33, 103),     S(27, 88),      S(12, 87),      S(16, 71),      S(15, 79),      S(-9, 107),     S(7, 91),
		S(18, 61),      S(-6, 65),      S(-14, 62),     S(-3, 57),      S(-9, 54),      S(-38, 60),     S(-8, 73),      S(-5, 61),
		S(7, 23),       S(-11, 33),     S(-7, 21),      S(-24, 27),     S(-4, 15),      S(10, 8),       S(24, 28),      S(27, 21),
		S(-4, 17),      S(5, 18),       S(2, 3),        S(6, 10),       S(-3, 9),       S(5, -4),       S(22, 9),       S(-5, 25),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};
constexpr Score BLOCKED_PASSED_PAWN_PENALTY[] = {
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
		S(-8, -133),    S(72, -181),    S(66, -166),    S(98, -211),    S(110, -189),   S(112, -269),   S(26, -268),    S(-4, -238),
		S(2, -109),     S(41, -141),    S(21, -137),    S(22, -117),    S(14, -101),    S(68, -140),    S(35, -170),    S(-64, -150),
		S(28, -69),     S(7, -74),      S(13, -66),     S(6, -55),      S(-5, -46),     S(-6, -66),     S(24, -73),     S(15, -94),
		S(18, -18),     S(17, -39),     S(-15, -33),    S(2, -30),      S(-22, -28),    S(26, -48),     S(-94, -36),    S(9, -34),
		S(4, -6),       S(9, -0),       S(-32, -9),     S(-1, -36),     S(-34, -14),    S(-39, -0),     S(-32, 25),     S(-81, -0),
		S(6, -19),      S(-4, -29),     S(-13, -6),     S(12, -62),     S(31, -116),    S(44, -23),     S(-100, 6),     S(14, -24),
		S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0)
};

// pretty ignore
template<Color color, PieceType piece_type>
constexpr Score read_psqt(Square square) {
	if constexpr (color == WHITE) square = flip(square);
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
// pretty stop-ignore
