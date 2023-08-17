#pragma once

#include "../../types.h"
#include "board_types.h"

constexpr u32 NPIECE_TYPES = 7;
class PieceType {
public:
	constexpr PieceType() : m_value{0} {}
	constexpr explicit PieceType(u32 v) : m_value{v} {}

	[[nodiscard]] constexpr operator u32() const { return m_value; }
	[[nodiscard]] constexpr bool operator==(const PieceType&) const = default;
	u32 m_value;
};
constexpr PieceType PAWN{0};
constexpr PieceType KNIGHT{1};
constexpr PieceType BISHOP{2};
constexpr PieceType ROOK{3};
constexpr PieceType QUEEN{4};
constexpr PieceType KING{5};
constexpr PieceType NO_PIECE_TYPE{6};

constexpr i8 NPIECES = 15;
class Piece {
public:
	constexpr Piece() : m_value{0} {}
	constexpr explicit Piece(u32 v) : m_value{v} {}

	[[nodiscard]] constexpr operator u32() const { return m_value; }
	[[nodiscard]] constexpr bool operator==(const Piece&) const = default;
	u32 m_value;
};
constexpr Piece WHITE_PAWN{0};
constexpr Piece WHITE_KNIGHT{1};
constexpr Piece WHITE_BISHOP{2};
constexpr Piece WHITE_ROOK{3};
constexpr Piece WHITE_QUEEN{4};
constexpr Piece WHITE_KING{5};
constexpr Piece BLACK_PAWN{8};
constexpr Piece BLACK_KNIGHT{9};
constexpr Piece BLACK_BISHOP{10};
constexpr Piece BLACK_ROOK{11};
constexpr Piece BLACK_QUEEN{12};
constexpr Piece BLACK_KING{13};
constexpr Piece NO_PIECE{14};

template<Color color, PieceType piece_type>
consteval Piece make_piece() {
	return static_cast<Piece>((color << 3) | piece_type);
}

inline Piece make_piece(Color color, PieceType piece_type) {
	return static_cast<Piece>((color << 3) | piece_type);
}

template<Piece piece>
consteval PieceType type_of() {
	return static_cast<PieceType>(piece & 0b000111);
}

inline PieceType type_of(Piece piece) {
	return static_cast<PieceType>(piece & 0b000111);
}

constexpr Color color_of(Piece pc) {
	return Color((pc & 0b1000) >> 3);
}

inline Piece piece_from_char(char c) {
	switch (c) {
		case 'P': return WHITE_PAWN;
		case 'N': return WHITE_KNIGHT;
		case 'B': return WHITE_BISHOP;
		case 'R': return WHITE_ROOK;
		case 'Q': return WHITE_QUEEN;
		case 'K': return WHITE_KING;
		case 'p': return BLACK_PAWN;
		case 'n': return BLACK_KNIGHT;
		case 'b': return BLACK_BISHOP;
		case 'r': return BLACK_ROOK;
		case 'q': return BLACK_QUEEN;
		case 'k': return BLACK_KING;
		default: return NO_PIECE;
	}
}