#pragma once
#include "../../board/types/piece.h"
#include "../types.h"
constexpr Score PIECE_VALUES[] = {
        S(99, 116),     S(456, 388),    S(467, 387),    S(657, 778),    S(1485, 1402),  S(0, 0),        
};
constexpr Score OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(42, -7),      S(-6, 16),      S(-53, 5),      
};
constexpr Score SEMI_OPEN_FILE_BONUS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(14, 8),       S(1, 9),        S(-20, 15),     
};
constexpr Score PAWN_PROTECTION[] = {
        S(7, 4),        S(1, 22),       S(2, 16),       S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ATTACKED_BY_PAWN[] = {
        S(0, 0),        S(-80, -39),    S(-74, -65),    S(35, -33),     S(-81, -5),     S(0, 0),        
};
constexpr Score THREATS[] = {
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(35, 44),      S(63, 28),      S(50, 3),       S(0, 0),
        S(0, 0),        S(27, 43),      S(0, 0),        S(51, 35),      S(54, 24),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(68, 13),      S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
};
constexpr Score KING_RING_ATTACK_BONUS[] = {
        S(21, -12),     S(30, -12),     S(23, -6),      S(34, -11),     S(31, 6),       S(0, 0),        
};
constexpr Score CHECK_BONUS[] = {
        S(207, 3),      S(126, 23),     S(77, 62),      S(125, 8),      S(71, 100),     S(0, 0),        
};
constexpr Score CENTER_CONTROL[] = {
        S(0, 0),        S(-4, -3),      S(3, 10),       S(1, 0),        S(-1, 10),      S(0, 0),        
};
constexpr Score KING_PAWN_SHIELD[] = {
        S(39, -20),     S(32, -16),     
};
constexpr Score KNIGHT_MOBILITY[] = {
        S(-6, -40),     S(8, 2),        S(13, 11),      S(12, 21),      S(15, 22),      S(13, 29),      S(16, 26),      S(16, 18),
        S(28, -3),      
};
constexpr Score BISHOP_MOBILITY[] = {
        S(-19, -45),    S(-5, -23),     S(4, -3),       S(8, 9),        S(13, 17),      S(18, 24),      S(18, 30),      S(15, 32),
        S(21, 34),      S(20, 30),      S(29, 26),      S(48, 17),      S(33, 35),      S(58, 0),       
};
constexpr Score ROOK_MOBILITY[] = {
        S(-23, -101),   S(-11, -58),    S(-10, -35),    S(-2, -20),     S(1, -3),       S(-5, 17),      S(0, 20),       S(7, 21),
        S(8, 29),       S(13, 32),      S(12, 38),      S(19, 41),      S(24, 41),      S(24, 37),      S(61, 20),      
};
constexpr Score QUEEN_MOBILITY[] = {
        S(-123, 140),   S(-11, -433),   S(-30, -123),   S(-25, -128),   S(-28, -29),    S(-23, -9),     S(-24, 10),     S(-22, 25),
        S(-23, 44),     S(-19, 40),     S(-17, 51),     S(-20, 63),     S(-16, 60),     S(-14, 60),     S(-9, 57),      S(-11, 63),
        S(-12, 57),     S(-4, 48),      S(-7, 51),      S(5, 39),       S(33, 7),       S(24, 20),      S(3, 18),       S(85, -15),
        S(20, -14),     S(46, -21),     S(178, -115),   S(364, -207),   
};
constexpr Score KNIGHT_FORWARD_MOBILITY[] = {
        S(-16, -16),    S(-8, 1),       S(1, 8),        S(11, 12),      S(25, 12),      S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        
};
constexpr Score BISHOP_FORWARD_MOBILITY[] = {
        S(-3, -7),      S(-1, 1),       S(3, 1),        S(5, 5),        S(9, 5),        S(7, 5),        S(9, 4),        S(15, 14),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score ROOK_FORWARD_MOBILITY[] = {
        S(-4, -7),      S(-1, -9),      S(-1, -1),      S(4, 0),        S(6, 6),        S(3, 8),        S(5, 13),       S(4, 30),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score QUEEN_FORWARD_MOBILITY[] = {
        S(-5, -29),     S(3, -20),      S(1, -11),      S(5, -26),      S(5, -13),      S(6, -17),      S(5, -6),       S(7, -4),
        S(5, -1),       S(2, 14),       S(9, 21),       S(-4, 41),      S(-21, 68),     S(-8, 112),     S(-23, 107),    S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),
        S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        
};
constexpr Score KING_LINE_SAFETY[] = {
        S(0, 0),        S(0, 0),        S(48, -31),     S(35, 12),      S(30, 11),      S(23, 6),       S(22, 4),       S(20, 4),
        S(9, 6),        S(2, 12),       S(1, 4),        S(-6, 5),       S(-16, 4),      S(-19, 3),      S(-52, 13),     S(-48, 8),
        S(-46, 4),      S(-58, 1),      S(-71, 2),      S(-66, -4),     S(-76, -5),     S(-106, -12),   S(-116, -12),   S(-131, -30),
        S(-90, -44),    S(-14, -80),    S(0, 0),        S(0, 0),        
};
constexpr Score PHALANX_PAWN[] = {
        S(0, 0),        S(2, -6),       S(18, 5),       S(29, 26),      S(51, 76),      S(70, 127),     S(-17, 442),    S(0, 0),
};
constexpr Score CANDIDATE_PASSED_PAWN[] = {
        S(0, 0),        S(-6, -4),      S(2, -3),       S(11, 10),      S(19, 18),      S(32, 61),      S(0, 0),        S(0, 0),
};
constexpr Score ISOLATED_PAWN_PENALTY = S(-12, -13);
constexpr Score DOUBLED_PAWN_PENALTY = S(-18, -23);
constexpr Score BISHOP_PAIR_BONUS = S(27, 75);
constexpr Score PAWNLESS_KING_FLANK = S(-38, -14);
constexpr Score TEMPO = S(26, 24);

