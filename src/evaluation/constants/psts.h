#pragma once
#include "move_generation/types.h"
#include "evaluation/types.h"
constexpr Score PAWN_TABLE[] = {
		S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0),
		S( 100,  153), S(  74,  155), S(  50,  152), S(  72,  132), S(  35,  138), S(  64,  140), S(  -8,  174), S(  32,  170),
		S( -13,   51), S(  -5,   36), S(  17,   40), S(  49,    0), S(  59,   55), S(  94,   15), S(  26,   33), S(  17,   44),
		S(  -8,   21), S(  -2,   14), S(   7,   15), S(  33,  -13), S(  25,    7), S(  38,   14), S(   5,   17), S(  -4,   24),
		S( -12,   14), S( -11,   14), S(   9,    3), S(  24,    1), S(  22,    6), S(  24,    0), S(   3,    5), S(  -8,    5),
		S( -12,    2), S( -18,    8), S(   0,    7), S(   9,    7), S(  12,   22), S(  -5,   20), S(  -8,    5), S( -24,    4),
		S( -11,   12), S(  -7,    9), S(  -8,   26), S(   4,   13), S(   2,   36), S(  10,   29), S(   6,    8), S( -31,    2),
		S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0),
};
constexpr Score KNIGHT_TABLE[] = {
		S(-207,  -12), S(-139,   17), S( -67,   39), S(  -6,   12), S(  98,    8), S(-137,   31), S( -41,  -17), S(-127,  -65),
		S( -26,   13), S( -37,   31), S(  65,   16), S(  29,   50), S(  21,   44), S(  82,    4), S( -15,   25), S(  -8,    1),
		S( -26,   11), S(  48,   22), S(  49,   62), S(  79,   61), S(  99,   39), S( 122,   41), S(  67,   19), S(  51,  -10),
		S(  29,   22), S(  39,   41), S(  46,   67), S(  78,   68), S(  46,   75), S(  75,   69), S(  38,   46), S(  52,   25),
		S(  14,   28), S(  19,   31), S(  40,   60), S(  49,   68), S(  59,   63), S(  49,   61), S(  73,   34), S(  25,   37),
		S(  -7,   -4), S(  13,   23), S(  31,   24), S(  33,   54), S(  51,   38), S(  43,   16), S(  38,    0), S(   6,   22),
		S( -16,  -13), S(  -5,    8), S(  15,   12), S(  35,   28), S(  34,   37), S(  33,    5), S(  14,   24), S(  32,  -16),
		S( -96,    5), S(  12,   -8), S( -15,   15), S(   4,   29), S(  30,   10), S(  17,   19), S(  12,    6), S( -19,  -33),
};
constexpr Score BISHOP_TABLE[] = {
		S( -27,   28), S(  13,   25), S(-102,   48), S( -56,   40), S( -52,   47), S( -82,   38), S(   6,   28), S(  14,   21),
		S( -30,   43), S(  24,   27), S(   6,   40), S( -26,   39), S(  30,   35), S(  49,   27), S(  11,   33), S( -30,   37),
		S(  16,   29), S(  34,   28), S(  64,   26), S(  33,   31), S(  57,   25), S(  71,   38), S(  74,   32), S(  27,   40),
		S(  -6,   34), S(  35,   35), S(  18,   46), S(  76,   37), S(  55,   38), S(  59,   34), S(  40,   46), S(  13,   45),
		S(   9,   27), S(  18,   35), S(  30,   44), S(  57,   44), S(  65,   34), S(  25,   38), S(  19,   27), S(  47,   14),
		S(  23,   18), S(  36,   34), S(  33,   36), S(  33,   37), S(  31,   47), S(  41,   31), S(  32,   23), S(  43,   14),
		S(  37,   10), S(  42,    9), S(  39,   19), S(  23,   32), S(  31,   29), S(  30,   10), S(  60,   29), S(  25,    4),
		S(  43,    3), S(  45,   21), S(  18,   50), S(  20,   27), S(   6,   33), S(  12,   44), S(  12,   27), S(  20,    3),
};
constexpr Score ROOK_TABLE[] = {
		S(  25,   54), S(  30,   53), S(  -9,   63), S(   4,   58), S(  39,   52), S(   6,   55), S(  34,   46), S(  60,   46),
		S(   6,   57), S(  10,   58), S(  31,   52), S(  41,   54), S(  66,   36), S(  92,   27), S(  30,   48), S(  50,   40),
		S( -12,   52), S(  24,   45), S(  34,   45), S(  42,   42), S(  62,   28), S(  99,   26), S( 144,    8), S(  48,   26),
		S( -10,   48), S(   3,   44), S(  24,   47), S(  45,   37), S(  28,   44), S(  49,   41), S(  28,   35), S(   7,   43),
		S( -25,   46), S( -22,   49), S( -12,   48), S(  -2,   44), S(   6,   41), S(  20,   28), S(  44,   26), S(   1,   28),
		S( -29,   30), S( -18,   36), S( -14,   28), S( -10,   31), S(  -6,   32), S(  15,   20), S(  31,   18), S(   4,   11),
		S( -37,   27), S( -11,   20), S( -17,   33), S(  -9,   27), S(   4,   24), S(  15,   15), S(  21,    9), S( -56,   30),
		S(  -9,   34), S(  -8,   31), S(  -6,   32), S(   6,   23), S(   6,   23), S(  12,   25), S(  -4,   24), S(   5,   12),
};
constexpr Score QUEEN_TABLE[] = {
		S( -12,  -15), S( -27,   42), S( -16,   44), S(  16,   10), S(  74,   18), S(  64,   11), S(  77,    2), S(  75,    4),
		S( -10,   13), S( -48,   41), S( -17,   57), S(  -9,   62), S( -30,   86), S(  31,   61), S(   7,   64), S(  63,   13),
		S(  -1,    2), S(  -1,   14), S(   6,   21), S( -32,   93), S(  -5,   89), S(  97,   41), S(  98,   19), S(  18,   78),
		S(  -8,    4), S(  -8,   37), S( -15,   52), S( -14,   69), S(  -5,   90), S(  27,   65), S(  31,   85), S(  16,   89),
		S(   4,  -12), S(   1,   34), S(  12,   18), S(  -1,   65), S(  12,   44), S(  20,   59), S(  31,   52), S(  26,   21),
		S( -11,    0), S(  32,  -50), S(  20,    2), S(  12,   -5), S(  18,   10), S(  24,   11), S(  42,   -7), S(  20,   27),
		S( -17,   -7), S(  19,  -54), S(  34,  -56), S(  22,  -16), S(  31,  -33), S(  34,  -52), S(  35,  -69), S(   4,  -33),
		S(  11,  -36), S(  -6,  -33), S(  10,  -35), S(  22,   -2), S(  10,  -35), S(   2,  -55), S(  17,  -59), S( -16,  -71),
};
constexpr Score KING_TABLE[] = {
		S(-108, -101), S( 192,  -85), S( 192,  -63), S( 120,  -57), S(-134,    1), S( -86,   24), S( 119,  -27), S(   2,  -38),
		S( 184,  -66), S( 109,    4), S( 103,    6), S( 119,   -1), S(  70,   12), S(  99,   19), S(   4,   34), S(-174,   22),
		S(  45,   -2), S( 115,   12), S( 126,   20), S(  50,   17), S(  92,   10), S( 146,   38), S( 182,   29), S( -26,   10),
		S(  -1,  -12), S(  52,   19), S(  30,   34), S(  17,   41), S( -18,   39), S(  31,   32), S(  15,   27), S(-118,   14),
		S( -91,  -13), S(  78,  -17), S(   3,   24), S( -42,   46), S( -44,   43), S( -30,   27), S( -31,   11), S( -98,   -7),
		S( -30,  -25), S( -11,    1), S( -28,   17), S( -40,   33), S( -34,   32), S( -28,   16), S( -15,    2), S( -43,  -19),
		S( -25,  -30), S( -38,   -9), S( -20,    9), S( -74,   23), S( -47,   19), S( -42,   13), S( -42,    2), S( -36,  -26),
		S( -53,  -79), S( -13,  -48), S( -54,  -11), S( -71,   -5), S( -79,   -9), S( -41,  -16), S( -24,  -31), S( -32,  -77),
};
constexpr Score PASSED_PAWN_BONUS[] = {
		S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0),
		S( 100,  153), S(  74,  155), S(  50,  152), S(  72,  132), S(  35,  138), S(  64,  140), S(  -8,  174), S(  32,  170),
		S(  70,  187), S(  51,  198), S(  50,  153), S(  19,  165), S(  15,   92), S( -14,  157), S( -16,  177), S(   5,  177),
		S(  23,  112), S(  14,  101), S(   9,   85), S(   1,   84), S(   3,   66), S(  22,   66), S( -13,  100), S(   4,   86),
		S(  11,   59), S(  -5,   58), S( -29,   52), S( -19,   43), S( -17,   45), S( -49,   54), S( -14,   65), S(  -1,   56),
		S(   0,   24), S( -21,   36), S( -17,   23), S( -41,   28), S( -15,   14), S(   7,    7), S(  24,   25), S(  31,   17),
		S(  -4,   20), S(  13,   19), S(  -3,    8), S( -13,   24), S( -36,   16), S(   6,    2), S(  27,   11), S(  -6,   28),
		S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0),
};
constexpr Score BLOCKED_PASSED_PAWN_PENALTY[] = {
		S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0),
		S(  16, -133), S( 113, -179), S(  81, -166), S( 144, -215), S( 152, -188), S( 130, -249), S(  79, -282), S(   1, -219),
		S(   3, -115), S(  49, -152), S(  24, -125), S(  22, -121), S(  10, -102), S( 113, -146), S(  -7, -148), S( -61, -143),
		S(  40,  -69), S(   7,  -66), S(  13,  -68), S(   1,  -44), S( -15,  -41), S( -12,  -65), S(   7,  -73), S(  18,  -87),
		S(  22,  -23), S(  -8,  -32), S( -23,  -28), S(  11,  -33), S(  -6,  -32), S(  43,  -44), S( -94,  -33), S(   4,  -29),
		S(  -4,   -2), S(   8,  -13), S( -58,    7), S(  16,  -48), S( -20,   -9), S(  33,  -17), S( -49,   37), S( -51,   -6),
		S(   9,  -36), S(   0,  -15), S( -19,   18), S( -51,  -31), S(  91, -159), S(  32,  -28), S( -61,    8), S(  39,  -33),
		S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0), S(   0,    0),
};

// pretty ignore
template<Color color, PieceType piece_type>
constexpr Score read_psqt(Square square) {
	if (color == WHITE) square = ~square;
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
