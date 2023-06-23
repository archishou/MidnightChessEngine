#pragma once
#include "../../board/types/piece.h"
#include "../types.h"
constexpr Score PIECE_VALUES[] = {
        S(97, 114),     S(453, 383),    S(464, 388),    S(647, 770),    S(1462, 1392),  S(0, 0),        
};
constexpr Score OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(42, -4),      S(-7, 26),      S(-53, 3),      
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(13, 11),      S(-1, 24),      S(-20, 13),     
};
constexpr Score PAWN_PROTECTION[] = {
        S(7, 4),        S(1, 23),       S(2, 19),       S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ATTACKED_BY_PAWN[] = {
        S(0, 0),        S(-80, -36),    S(-74, -65),    S(76, -66),     S(-81, -11),    S(0, 0),        
};
constexpr Score THREATS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(34, 47),      S(62, 26),      S(51, 4),       S(0, 0),
        S(0, 0),        S(28, 39),      S(0, 0),        S(51, 32),      S(54, 24),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(68, 11),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
        S(22, -11),     S(30, -12),     S(23, -6),      S(34, -10),     S(31, 7),       S(0, 0),        
};
constexpr Score CHECK_BONUS[] = {
        S(207, 5),      S(123, 23),     S(76, 63),      S(125, 7),      S(66, 105),     S(0, 0),        
};
constexpr Score CENTER_CONTROL[] = {
        S(0, 0),        S(-4, 3),       S(3, 11),       S(0, 1),        S(-2, 15),      S(0, 0),        
};
constexpr Score KING_PAWN_SHIELD[] = {
        S(38, -18),     S(31, -11),     
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -11);
constexpr Score DOUBLED_PAWN_PENALTY = S(-17, -24);
constexpr Score BISHOP_PAIR_BONUS = S(26, 74);
constexpr Score KNIGHT_MOBILITY[] = {
        S(-12, 23),     S(6, 21),       S(12, 10),      S(11, 15),      S(14, 16),      S(13, 20),      S(15, 16),      S(15, 8),
        S(27, -15),     
};
constexpr Score BISHOP_MOBILITY[] = {
        S(-26, 34),     S(-8, -14),     S(2, -7),       S(7, -2),       S(12, 7),       S(17, 15),      S(16, 22),      S(14, 25),
        S(20, 27),      S(19, 25),      S(29, 21),      S(47, 12),      S(34, 29),      S(57, -5),      
};
constexpr Score ROOK_MOBILITY[] = {
        S(-28, -46),    S(-14, -31),    S(-12, -13),    S(-2, -24),     S(1, -8),       S(-5, 14),      S(1, 12),       S(8, 12),
        S(9, 22),       S(14, 25),      S(14, 30),      S(21, 32),      S(26, 33),      S(26, 27),      S(62, 11),      
};
constexpr Score QUEEN_MOBILITY[] = {
        S(-124, 112),   S(-32, -112),   S(-47, 47),     S(-40, -2),     S(-43, 87),     S(-36, 57),     S(-36, 72),     S(-31, 55),
        S(-29, 42),     S(-22, 23),     S(-19, 28),     S(-21, 31),     S(-15, 24),     S(-11, 17),     S(-5, 12),      S(-7, 19),
        S(-8, 12),      S(3, -2),       S(0, -1),       S(14, -15),     S(42, -49),     S(37, -39),     S(17, -43),     S(102, -79),
        S(23, -72),     S(69, -89),     S(186, -179),   S(354, -259),   
};
constexpr Score KNIGHT_FORWARD_MOBILITY[] = {
        S(-17, -27),    S(-10, -4),     S(-1, 4),       S(9, 9),        S(24, 7),       S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        
};
constexpr Score BISHOP_FORWARD_MOBILITY[] = {
        S(-4, -9),      S(-3, 3),       S(1, 1),        S(4, 5),        S(7, 3),        S(5, 3),        S(8, 0),        S(12, 11),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ROOK_FORWARD_MOBILITY[] = {
        S(-5, -2),      S(-2, -9),      S(-1, -5),      S(4, -1),       S(6, 5),        S(3, 7),        S(5, 13),       S(5, 27),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score QUEEN_FORWARD_MOBILITY[] = {
        S(-2, -37),     S(5, -19),      S(2, -7),       S(7, -26),      S(4, -4),       S(4, -6),       S(4, 1),        S(6, 2),
        S(3, 2),        S(-1, 14),      S(6, 19),       S(-7, 34),      S(-27, 61),     S(-15, 100),    S(-30, 82),     S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score KING_LINE_SAFETY[] = {
        S(0, 0),        S(0, 0),        S(50, -38),     S(36, 13),      S(31, 9),       S(23, 6),       S(23, 3),       S(21, 5),
        S(11, 6),       S(3, 14),       S(2, 5),        S(-4, 7),       S(-14, 6),      S(-18, 5),      S(-51, 15),     S(-47, 10),
        S(-46, 6),      S(-61, 3),      S(-74, 4),      S(-73, -2),     S(-83, -4),     S(-113, -11),   S(-126, -12),   S(-140, -32),
        S(-103, -48),   S(-25, -85),    S(0, 0),        S(0, 0),        
};
constexpr Score PHALANX_PAWN[] = {
        S(0, 0),        S(2, -2),       S(18, 5),       S(29, 25),      S(50, 77),      S(66, 128),     S(-5, 409),     S(0, 0),
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
        S(0, 0),        S(-5, -7),      S(3, -5),       S(11, 11),      S(19, 18),      S(32, 58),      S(0, 0),        S(0, 0),
};
constexpr Score TEMPO = S(26, 23);

