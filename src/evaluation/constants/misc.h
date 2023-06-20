#pragma once
#include "../../board/types/piece.h"
#include "../types.h"
constexpr Score PIECE_VALUES[] = {
        S(97, 113),     S(452, 382),    S(461, 386),    S(642, 764),    S(1451, 1382),  S(0, 0),        
};
constexpr Score OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(41, -4),      S(-7, 26),      S(-52, 3),      
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(13, 11),      S(-1, 23),      S(-19, 13),     
};
constexpr Score PAWN_PROTECTION[] = {
        S(7, 4),        S(1, 22),       S(2, 19),       S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ATTACKED_BY_PAWN[] = {
        S(0, 0),        S(-79, -35),    S(-73, -64),    S(75, -66),     S(-80, -11),    S(0, 0),        
};
constexpr Score THREATS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(34, 46),      S(61, 26),      S(51, 4),       S(0, 0),
        S(0, 0),        S(27, 39),      S(0, 0),        S(51, 32),      S(54, 24),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(68, 11),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
        S(22, -11),     S(29, -12),     S(23, -6),      S(34, -10),     S(31, 7),       S(0, 0),        
};
constexpr Score CHECK_BONUS[] = {
        S(206, 5),      S(122, 23),     S(75, 62),      S(124, 7),      S(66, 104),     S(0, 0),        
};
constexpr Score CENTER_CONTROL[] = {
        S(0, 0),        S(-4, 3),       S(3, 11),       S(0, 1),        S(-2, 15),      S(0, 0),        
};
constexpr Score KING_PAWN_SHIELD[] = {
        S(38, -18),     S(31, -11),     
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -11);
constexpr Score DOUBLED_PAWN_PENALTY = S(-17, -24);
constexpr Score BISHOP_PAIR_BONUS = S(25, 73);
constexpr Score KNIGHT_MOBILITY[] = {
        S(-13, 22),     S(4, 20),       S(11, 9),       S(10, 13),      S(13, 14),      S(11, 19),      S(14, 14),      S(14, 6),
        S(26, -16),     
};
constexpr Score BISHOP_MOBILITY[] = {
        S(-26, 34),     S(-8, -14),     S(2, -7),       S(6, -2),       S(11, 7),       S(16, 14),      S(16, 22),      S(13, 25),
        S(19, 26),      S(19, 24),      S(28, 21),      S(47, 12),      S(33, 28),      S(56, -5),      
};
constexpr Score ROOK_MOBILITY[] = {
        S(-28, -46),    S(-14, -31),    S(-12, -13),    S(-2, -24),     S(1, -8),       S(-5, 14),      S(1, 12),       S(8, 12),
        S(9, 21),       S(14, 25),      S(14, 30),      S(21, 32),      S(26, 32),      S(26, 27),      S(61, 11),      
};
constexpr Score QUEEN_MOBILITY[] = {
        S(-123, 107),   S(-32, -111),   S(-47, 46),     S(-39, -2),     S(-43, 87),     S(-36, 57),     S(-36, 71),     S(-31, 54),
        S(-29, 42),     S(-22, 22),     S(-19, 28),     S(-21, 30),     S(-15, 24),     S(-11, 17),     S(-5, 12),      S(-7, 19),
        S(-8, 12),      S(3, -2),       S(0, -1),       S(13, -15),     S(42, -49),     S(36, -39),     S(16, -43),     S(101, -79),
        S(23, -71),     S(68, -88),     S(182, -176),   S(351, -257),   
};
constexpr Score KNIGHT_FORWARD_MOBILITY[] = {
        S(-18, -27),    S(-11, -4),     S(-2, 3),       S(8, 8),        S(23, 6),       S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        
};
constexpr Score BISHOP_FORWARD_MOBILITY[] = {
        S(-5, -10),     S(-3, 3),       S(0, -0),       S(3, 4),        S(6, 3),        S(4, 3),        S(7, -1),       S(11, 10),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ROOK_FORWARD_MOBILITY[] = {
        S(-5, -2),      S(-2, -9),      S(-1, -4),      S(4, -1),       S(5, 5),        S(3, 7),        S(5, 13),       S(5, 27),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score QUEEN_FORWARD_MOBILITY[] = {
        S(-2, -37),     S(5, -20),      S(2, -8),       S(6, -26),      S(4, -5),       S(4, -7),       S(3, 1),        S(5, 1),
        S(2, 2),        S(-1, 13),      S(5, 18),       S(-7, 34),      S(-27, 60),     S(-15, 98),     S(-30, 81),     S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score KING_LINE_SAFETY[] = {
        S(0, 0),        S(0, 0),        S(50, -37),     S(36, 13),      S(30, 9),       S(23, 6),       S(23, 3),       S(21, 5),
        S(11, 6),       S(3, 14),       S(2, 6),        S(-4, 7),       S(-14, 6),      S(-18, 6),      S(-51, 16),     S(-47, 10),
        S(-46, 6),      S(-60, 3),      S(-74, 4),      S(-72, -2),     S(-82, -3),     S(-112, -11),   S(-125, -11),   S(-139, -31),
        S(-102, -48),   S(-25, -84),    S(0, 0),        S(0, 0),        
};
constexpr Score PHALANX_PAWN[] = {
        S(0, 0),        S(2, -2),       S(18, 5),       S(29, 25),      S(50, 77),      S(65, 127),     S(-5, 406),     S(0, 0),
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
        S(0, 0),        S(-5, -7),      S(3, -5),       S(11, 10),      S(19, 18),      S(31, 58),      S(0, 0),        S(0, 0),
};
constexpr Score TEMPO = S(26, 23);

