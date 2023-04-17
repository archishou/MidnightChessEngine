#pragma once

#include <cstdint>
#include <string>
#include <ostream>
#include <iostream>
#include <vector>

const int NCOLORS = 2;
enum Color : int {
	WHITE, BLACK
};

constexpr Color operator~(Color c) {
	return Color(c ^ BLACK);
}

const int NDIRS = 8;
enum Direction : int {
	NORTH = 8, NORTH_EAST = 9, EAST = 1, SOUTH_EAST = -7,
	SOUTH = -8, SOUTH_WEST = -9, WEST = -1, NORTH_WEST = 7,
	NORTH_NORTH = 16, SOUTH_SOUTH = -16
};

typedef uint64_t ZobristHash;

const int NPIECE_TYPES = 6;
enum PieceType : int {
	PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

constexpr int NO_PIECE_TYPE = 0b110;

static const std::string PIECE_STR = "PNBRQK~>pnbrqk.";

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";

constexpr auto  KIWIPETE = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";

const int NCASTLING_RIGHTS = 16;

const int NPIECES = 15;
enum Piece : int {
	WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN = 8, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING,
	NO_PIECE
};

constexpr Piece make_piece(Color c, PieceType pt) {
	return Piece((c << 3) + pt);
}

constexpr PieceType type_of(Piece pc) {
	return PieceType(pc & 0b111);
}

constexpr Color color_of(Piece pc) {
	return Color((pc & 0b1000) >> 3);
}

typedef uint64_t Bitboard;

const int NSQUARES = 64;
enum Square : int {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,
	NO_SQUARE
};

inline Square& operator++(Square& s) { return s = Square(static_cast<int>(s) + 1); }
constexpr Square operator+(Square s, Direction d) { return Square(static_cast<int>(s) + static_cast<int>(d)); }
constexpr Square operator-(Square s, Direction d) { return Square(static_cast<int>(s) - static_cast<int>(d)); }
constexpr Square operator~(Square& s) {return Square(static_cast<int>(s) ^ 56); }
inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }

const int NFILES = 8;

using File = int;

constexpr File AFILE = 0;
constexpr File BFILE = 1;
constexpr File CFILE = 2;
constexpr File DFILE = 3;
constexpr File EFILE = 4;
constexpr File FFILE = 5;
constexpr File GFILE = 6;
constexpr File HFILE = 7;

const int NRANKS = 8;

using Rank = int;

constexpr Rank RANK1 = 0;
constexpr Rank RANK2 = 1;
constexpr Rank RANK3 = 2;
constexpr Rank RANK4 = 3;
constexpr Rank RANK5 = 4;
constexpr Rank RANK6 = 5;
constexpr Rank RANK7 = 6;
constexpr Rank RANK8 = 7;

extern const char* SQSTR[65];

extern const Bitboard MASK_FILE[8];
extern const Bitboard MASK_RANK[8];
extern const Bitboard MASK_DIAGONAL[15];
extern const Bitboard MASK_ANTI_DIAGONAL[15];
extern const Bitboard SQUARE_BB[65];

extern void print_bitboard(Bitboard b);

extern const Bitboard k1;
extern const Bitboard k2;
extern const Bitboard k4;
extern const Bitboard kf;

extern /*inline*/ int pop_count(Bitboard x);
extern /*inline*/ int sparse_pop_count(Bitboard x);
extern /*inline*/ Square pop_lsb(Bitboard* b);

extern const int DEBRUIJN64[64];
extern const Bitboard MAGIC;
extern /*constexpr*/ Square bsf(Bitboard b);

constexpr Rank rank_of(Square s) { return Rank(s >> 3); }
constexpr File file_of(Square s) { return File(s & 0b111); }
constexpr int diagonal_of(Square s) { return 7 + rank_of(s) - file_of(s); }
constexpr int anti_diagonal_of(Square s) { return rank_of(s) + file_of(s); }
constexpr Square create_square(File f, Rank r) { return Square(r << 3 | f); }

template<Direction D>
constexpr Bitboard shift(Bitboard b) {
	return D == NORTH ? b << 8
		: D == SOUTH ? b >> 8
		: D == NORTH + NORTH ? b << 16
		: D == SOUTH + SOUTH ? b >> 16
		: D == EAST ? (b & ~MASK_FILE[HFILE]) << 1
		: D == WEST ? (b & ~MASK_FILE[AFILE]) >> 1
		: D == NORTH_EAST ? (b & ~MASK_FILE[HFILE]) << 9 
		: D == NORTH_WEST ? (b & ~MASK_FILE[AFILE]) << 7
		: D == SOUTH_EAST ? (b & ~MASK_FILE[HFILE]) >> 7 
		: D == SOUTH_WEST ? (b & ~MASK_FILE[AFILE]) >> 9
		: 0;	
}

template<Direction D>
constexpr Bitboard fill(Bitboard b) {
	switch (D) {
		case NORTH:
			b |= (b << 8);
			b |= (b << 16);
			return b | (b << 32);
		case SOUTH:
			b |= (b >> 8);
			b |= (b >> 16);
			return b | (b >> 32);
		default: return b;
	}
}

template<Color C>
constexpr Rank relative_rank(Rank r) {
	return C == WHITE ? r : Rank(RANK8 - r);
}

template<Color C>
constexpr Direction relative_dir(Direction d) {
	return Direction(C == WHITE ? d : -d);
}

template<Color C>
constexpr Bitboard forward_ranks(const Square s) {
	return C == WHITE ? ~MASK_RANK[RANK1] << 8 * relative_rank<WHITE>(rank_of(s))
					  : ~MASK_RANK[RANK8] >> 8 * relative_rank<BLACK>(rank_of(s));
}

template<Color C>
constexpr Bitboard forward_file(const Square s) {
	return forward_ranks<C>(s) & MASK_FILE[file_of(s)];
}

template<Color C>
constexpr Bitboard forward_files(const Bitboard b) {
	return C == WHITE ? fill<NORTH>(b) : fill<SOUTH>(b);
}

constexpr Bitboard file_fill(const Bitboard b) {
	return fill<NORTH>(b) | fill<SOUTH>(b);
}

const int NMOVEGENS = 64;
enum MoveGenType : int {
	ALL, QSEARCH
};

//The type of the move
enum MoveFlag : int {
	QUIET = 0b0000, DOUBLE_PUSH = 0b0001,
	OO = 0b0010, OOO = 0b0011,
	CAPTURE = 0b1000,
	CAPTURES = 0b1111,
	EN_PASSANT = 0b1010,
	PROMOTIONS = 0b0111,
	PROMOTION_CAPTURES = 0b1100,
	PR_KNIGHT = 0b0100, PR_BISHOP = 0b0101, PR_ROOK = 0b0110, PR_QUEEN = 0b0111,
	PC_KNIGHT = 0b1100, PC_BISHOP = 0b1101, PC_ROOK = 0b1110, PC_QUEEN = 0b1111,
};


class Move {
private:
	uint16_t move;
public:
	inline Move() : move(0) {}
	
	inline explicit Move(uint16_t m) { move = m; }

	inline Move(Square from, Square to) : move(0) {
		move = (from << 6) | to;
	}

	inline Move(Square from, Square to, MoveFlag flags) : move(0) {
		move = (flags << 12) | (from << 6) | to;
	}

	explicit Move(const std::string& m) {
		this->move = (create_square(File(m[0] - 'a'), Rank(m[1] - '1')) << 6) |
			create_square(File(m[2] - 'a'), Rank(m[3] - '1'));
	}

	[[nodiscard]] inline Square to() const { return Square(move & 0x3f); }
	[[nodiscard]] inline Square from() const { return Square((move >> 6) & 0x3f); }
	[[nodiscard]] inline int to_from() const { return move & 0xffff; }
	[[nodiscard]] inline MoveFlag flag() const { return MoveFlag((move >> 12) & 0xf); }

	[[nodiscard]] inline bool is_capture() const {
		return (move >> 12) & CAPTURE;
	}

	[[nodiscard]] inline bool is_promotion() const {
		MoveFlag flag = this->flag();
		return flag == PR_KNIGHT || flag == PR_BISHOP || flag == PR_ROOK || flag == PR_QUEEN ||
			flag == PC_KNIGHT || flag == PC_BISHOP || flag == PC_ROOK || flag == PC_QUEEN;
	}

	[[nodiscard]] inline bool is_quiet() const {
		return !is_capture() && !is_promotion();
	}

	bool operator==(Move a) const { return to_from() == a.to_from(); }
	bool operator!=(Move a) const { return to_from() != a.to_from(); }
};

template<MoveFlag F = QUIET>
inline Move *make(Square from, Bitboard to, Move *list) {
	while (to) *list++ = Move(from, pop_lsb(&to), F);
	return list;
}

template<>
inline Move *make<PROMOTIONS>(Square from, Bitboard to, Move *list) {
	Square p;
	while (to) {
		p = pop_lsb(&to);
		*list++ = Move(from, p, PR_KNIGHT);
		*list++ = Move(from, p, PR_BISHOP);
		*list++ = Move(from, p, PR_ROOK);
		*list++ = Move(from, p, PR_QUEEN);
	}
	return list;
}

template<>
inline Move* make<PROMOTION_CAPTURES>(Square from, Bitboard to, Move* list) {
	Square p;
	while (to) {
		p = pop_lsb(&to);
		*list++ = Move(from, p, PC_KNIGHT);
		*list++ = Move(from, p, PC_BISHOP);
		*list++ = Move(from, p, PC_ROOK);
		*list++ = Move(from, p, PC_QUEEN);
	}
	return list;
}

extern std::ostream& operator<<(std::ostream& os, const Move& m);

//The white king and kingside rook
const Bitboard WHITE_OO_MASK = 0x90;
//The white king and queenside rook
const Bitboard WHITE_OOO_MASK = 0x11;

//Squares between the white king and kingside rook
const Bitboard WHITE_OO_BLOCKERS_AND_ATTACKERS_MASK = 0x60;
//Squares between the white king and queenside rook
const Bitboard WHITE_OOO_BLOCKERS_AND_ATTACKERS_MASK = 0xe;

//The black king and kingside rook
const Bitboard BLACK_OO_MASK = 0x9000000000000000;
//The black king and queenside rook
const Bitboard BLACK_OOO_MASK = 0x1100000000000000;
//Squares between the black king and kingside rook
const Bitboard BLACK_OO_BLOCKERS_AND_ATTACKERS_MASK = 0x6000000000000000;
//Squares between the black king and queenside rook
const Bitboard BLACK_OOO_BLOCKERS_AND_ATTACKERS_MASK = 0xE00000000000000;

//The white king, white rooks, black king and black rooks
const Bitboard ALL_CASTLING_MASK = 0x9100000000000091;

template<Color C> constexpr Bitboard oo_mask() { return C == WHITE ? WHITE_OO_MASK : BLACK_OO_MASK; }
template<Color C> constexpr Bitboard ooo_mask() { return C == WHITE ? WHITE_OOO_MASK : BLACK_OOO_MASK; }

template<Color C>
constexpr Bitboard oo_blockers_mask() { 
	return C == WHITE ? WHITE_OO_BLOCKERS_AND_ATTACKERS_MASK :
		BLACK_OO_BLOCKERS_AND_ATTACKERS_MASK; 
}

template<Color C>
constexpr Bitboard ooo_blockers_mask() {
	return C == WHITE ? WHITE_OOO_BLOCKERS_AND_ATTACKERS_MASK :
		BLACK_OOO_BLOCKERS_AND_ATTACKERS_MASK;
}
	
template<Color C> constexpr Bitboard ignore_ooo_danger() { return C == WHITE ? 0x2 : 0x200000000000000; }
