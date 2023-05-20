#pragma once

#include "../board/types/bitboard.h"
#include "../board/position.h"

constexpr Bitboard BOARD_CENTER = 0x3c3c3c3c0000;

template<Direction D>
constexpr Bitboard fill(Bitboard b) {
	if constexpr (D == NORTH) {
		b |= (b << 8);
		b |= (b << 16);
		return b | (b << 32);
	} else if constexpr (D == SOUTH) {
		b |= (b >> 8);
		b |= (b >> 16);
		return b | (b >> 32);
	}
}

template<Color C>
constexpr Bitboard forward_files(const Bitboard b) {
	if constexpr (C == WHITE) return fill<NORTH>(b);
	return fill<SOUTH>(b);
}

constexpr Bitboard file_fill(const Bitboard b) {
	return fill<NORTH>(b) | fill<SOUTH>(b);
}

template<Color C>
constexpr Bitboard pawn_attacks(const Bitboard p) {
	if constexpr (C == WHITE) return shift<NORTH_WEST>(p) | shift<NORTH_EAST>(p);
	return shift<SOUTH_WEST>(p) | shift<SOUTH_EAST>(p);
}

template<Color color>
constexpr Bitboard pawn_attack_span(const Bitboard s) {
	return shift_relative<color, NORTH> (
			shift<EAST>(forward_files<color>(s)) |
			shift<WEST>(forward_files<color>(s))
	);
}

template<Color color>
constexpr Bitboard pawn_attack_files(const Bitboard s) {
	const Bitboard attacks = shift<WEST>(s) | shift<EAST>(s);
	return fill<SOUTH>(fill<NORTH>(attacks));
}

template<Color color>
inline Bitboard isolated_pawns(const Position& board) {
	Bitboard us_pawns = board.occupancy<color, PAWN>();
	return us_pawns & ~pawn_attack_files<color>(us_pawns);
}

template<Color color>
inline Bitboard doubled_pawns(const Position& board) {
	Bitboard us_pawns = board.occupancy<color, PAWN>();
	return us_pawns & shift_relative<color, SOUTH>(us_pawns);
}

template<Color color>
constexpr Bitboard pawn_passed_span(const Bitboard s) {
	return forward_files<color>(s) | pawn_attack_span<color>(s);
}

template<Color c>
inline Bitboard passed_pawns(const Position& board) {
	Bitboard them_pawn_occupants = pawn_passed_span<~c>(board.occupancy<~c, PAWN>());
	return board.occupancy<c, PAWN>() & ~them_pawn_occupants;
}

inline Bitboard open_files(const Position& board) {
	return ~file_fill(board.occupancy<WHITE, PAWN>()) & ~file_fill(board.occupancy<BLACK, PAWN>());
}

template<Color c>
inline Bitboard semi_open_files(const Position& board) {
	return ~file_fill(board.occupancy<c, PAWN>()) ^ open_files(board);
}

template<Color c>
inline Bitboard phalanx_pawns(const Position& board) {
	return board.occupancy<c, PAWN>() & shift<WEST>(board.occupancy<c, PAWN>());
}

template<Color c>
constexpr Bitboard candidate_pawns(const Position& board, Bitboard passed_pawns) {
	const Bitboard them_pawn_spawn = file_fill(board.occupancy<~c, PAWN>());
	return (board.occupancy<c, PAWN>() ^ passed_pawns) & ~them_pawn_spawn;
}