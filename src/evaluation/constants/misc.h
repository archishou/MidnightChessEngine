#pragma once
#include "../../board/types/piece.h"
#include "../types.h"
constexpr Score PIECE_VALUES[] = {
        S(99, 115),     S(456, 384),    S(466, 389),    S(655, 778),    S(1476, 1405),  S(0, 0),        
};
constexpr Score OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(42, -4),      S(-7, 27),      S(-53, 5),      
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(14, 10),      S(-1, 24),      S(-20, 16),     
};
constexpr Score PAWN_PROTECTION[] = {
        S(7, 4),        S(1, 23),       S(2, 19),       S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ATTACKED_BY_PAWN[] = {
        S(0, 0),        S(-81, -36),    S(-74, -65),    S(76, -67),     S(-81, -11),    S(0, 0),        
};
constexpr Score THREATS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(34, 47),      S(63, 27),      S(52, 4),       S(0, 0),
        S(0, 0),        S(28, 40),      S(0, 0),        S(52, 33),      S(55, 25),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(69, 11),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
        S(23, -11),     S(30, -12),     S(23, -6),      S(34, -10),     S(32, 7),       S(0, 0),        
};
constexpr Score CHECK_BONUS[] = {
        S(208, 4),      S(125, 23),     S(77, 63),      S(124, 8),      S(67, 106),     S(0, 0),        
};
constexpr Score CENTER_CONTROL[] = {
        S(0, 0),        S(-4, 3),       S(3, 11),       S(0, 1),        S(-2, 15),      S(0, 0),        
};
constexpr Score KING_PAWN_SHIELD[] = {
        S(38, -19),     S(31, -12),     
};
constexpr Score KNIGHT_MOBILITY[] = {
        S(-12, 25),     S(6, 22),       S(12, 11),      S(12, 16),      S(15, 17),      S(13, 21),      S(16, 17),      S(16, 9),
        S(28, -14),     
};
constexpr Score BISHOP_MOBILITY[] = {
        S(-25, 36),     S(-6, -12),     S(3, -6),       S(8, -0),       S(13, 9),       S(18, 16),      S(18, 23),      S(15, 26),
        S(21, 28),      S(20, 26),      S(30, 22),      S(49, 14),      S(35, 30),      S(59, -4),      
};
constexpr Score ROOK_MOBILITY[] = {
        S(-28, -47),    S(-15, -31),    S(-12, -14),    S(-2, -25),     S(1, -8),       S(-5, 14),      S(1, 12),       S(8, 12),
        S(9, 22),       S(14, 25),      S(14, 30),      S(22, 32),      S(26, 33),      S(26, 28),      S(63, 11),      
};
constexpr Score QUEEN_MOBILITY[] = {
        S(-128, 135),   S(-32, -114),   S(-47, 47),     S(-40, -2),     S(-44, 88),     S(-36, 58),     S(-36, 72),     S(-31, 55),
        S(-29, 43),     S(-22, 23),     S(-19, 29),     S(-21, 31),     S(-15, 25),     S(-11, 17),     S(-5, 12),      S(-7, 19),
        S(-8, 12),      S(3, -2),       S(0, -0),       S(14, -14),     S(42, -48),     S(37, -40),     S(17, -43),     S(104, -80),
        S(24, -72),     S(70, -91),     S(190, -182),   S(369, -270),   
};
constexpr Score KNIGHT_FORWARD_MOBILITY[] = {
        S(-16, -26),    S(-8, -3),      S(0, 5),        S(11, 11),      S(26, 9),       S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        
};
constexpr Score BISHOP_FORWARD_MOBILITY[] = {
        S(-3, -8),      S(-1, 5),       S(2, 2),        S(5, 7),        S(8, 5),        S(6, 5),        S(9, 2),        S(14, 12),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ROOK_FORWARD_MOBILITY[] = {
        S(-5, -2),      S(-1, -9),      S(-1, -5),      S(4, -1),       S(6, 5),        S(4, 7),        S(5, 13),       S(5, 27),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score QUEEN_FORWARD_MOBILITY[] = {
        S(-1, -37),     S(6, -19),      S(3, -7),       S(8, -25),      S(5, -3),       S(5, -6),       S(4, 2),        S(6, 3),
        S(3, 3),        S(-1, 15),      S(6, 20),       S(-6, 36),      S(-26, 62),     S(-14, 102),    S(-29, 83),     S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score KING_LINE_SAFETY[] = {
        S(0, 0),        S(0, 0),        S(49, -40),     S(35, 13),      S(30, 9),       S(23, 5),       S(22, 2),       S(20, 4),
        S(10, 5),       S(2, 12),       S(2, 4),        S(-5, 6),       S(-15, 5),      S(-19, 4),      S(-52, 15),     S(-48, 9),
        S(-47, 5),      S(-60, 2),      S(-74, 4),      S(-70, -2),     S(-80, -3),     S(-110, -11),   S(-121, -11),   S(-133, -30),
        S(-92, -44),    S(-15, -80),    S(0, 0),        S(0, 0),        
};
constexpr Score PHALANX_PAWN[] = {
        S(0, 0),        S(2, -2),       S(18, 5),       S(29, 25),      S(51, 78),      S(70, 127),     S(-6, 417),     S(0, 0),
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
        S(0, 0),        S(-5, -7),      S(3, -5),       S(11, 10),      S(19, 18),      S(32, 59),      S(0, 0),        S(0, 0),
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -12);
constexpr Score DOUBLED_PAWN_PENALTY = S(-18, -23);
constexpr Score BISHOP_PAIR_BONUS = S(26, 75);
constexpr Score PAWNLESS_KING_FLANK = S(-38, -14);
constexpr Score TEMPO = S(26, 23);

