#pragma once

#include "move_generation/position.h"

constexpr Bitboard BOARD_CENTER = 0x3c3c3c3c0000;

template<Color color>
constexpr Bitboard pawn_attack_span(const Bitboard s) {
	return shift<relative_dir<color>(NORTH)> (
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
inline Bitboard isolated_pawns(Position& board) {
	Bitboard us_pawns = board.bitboard_of(color, PAWN);
	return us_pawns & ~pawn_attack_files<color>(us_pawns);
}

template<Color color>
inline Bitboard doubled_pawns(Position& board) {
	Bitboard us_pawns = board.bitboard_of(color, PAWN);
	return us_pawns & shift<relative_dir<color>(SOUTH)>(us_pawns);
}

template<Color color>
constexpr Bitboard pawn_passed_span(const Bitboard s) {
	return forward_files<color>(s) | pawn_attack_span<color>(s);
}

template<Color c>
inline Bitboard passed_pawns(Position& board) {
	Bitboard them_pawn_occupants = pawn_passed_span<~c>(board.bitboard_of(~c, PAWN));
	return board.bitboard_of(c, PAWN) & ~them_pawn_occupants;
}

inline Bitboard open_files(Position& board) {
	return ~file_fill(board.bitboard_of(WHITE, PAWN)) & ~file_fill(board.bitboard_of(BLACK, PAWN));
}

template<Color c>
inline Bitboard semi_open_files(Position& board) {
	return ~file_fill(board.bitboard_of(c, PAWN)) ^ open_files(board);
}

template<Color c>
inline Bitboard phalanx_pawns(Position& board) {
	return board.bitboard_of(c, PAWN) & shift<WEST>(board.bitboard_of(c, PAWN));
}

template<Color c>
constexpr Bitboard candidate_pawns(Position& board, Bitboard passed_pawns) {
	const Bitboard them_pawn_spawn = file_fill(board.bitboard_of(~c, PAWN));
	return (board.bitboard_of(c, PAWN) ^ passed_pawns) & ~them_pawn_spawn;
}