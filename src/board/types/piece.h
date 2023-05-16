#pragma once

#include "../../types.h"
#include "board_types.h"

constexpr u32 NPIECE_TYPES = 7;
enum PieceType : u32 {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NO_PIECE_TYPE
};

constexpr i8 NPIECES = 15;
enum Piece : u32 {
	WHITE_PAWN,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN = 8,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING,
	NO_PIECE,
};

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