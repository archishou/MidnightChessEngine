//
// Created by Alex Tian on 12/2/2022.
//
#pragma once
#include <string>
#include "constants/misc_constants.h"
#include "../types.h"
#include "types/bitboard.h"
#include "constants/misc_constants.h"
#include "../move_gen/types/move.h"
#include "types/board_types.h"
#include "../utils/stack.h"
#include "../move_gen/tables/attack_tables.h"

class Position;

class PositionState {
	friend Position;
private:
	static constexpr Bitboard WHITE_OO_BANNED_MASK		= 0x90;
	static constexpr Bitboard WHITE_OOO_BANNED_MASK		= 0x11;

	static constexpr Bitboard BLACK_OO_BANNED_MASK		= 0x9000000000000000;
	static constexpr Bitboard BLACK_OOO_BANNED_MASK		= 0x1100000000000000;

	static constexpr Bitboard NO_CASTLING_MASK = 0x9100000000000091;

	Bitboard from_to{};
	Piece captured{};
	Square ep_square = NO_SQUARE;
	u16 fifty_move_rule{};
public:
	ZobristHash hash{};

public:
	PositionState() = default;
	~PositionState() = default;
};

class Position {
private:
	Color side = WHITE;

	std::array<Bitboard, NPIECES> pieces{};
	std::array<Piece, NSQUARES> board{};

	static constexpr i16 POSITION_STATE_SIZE = 1000;
	Stack<PositionState, POSITION_STATE_SIZE> state_history{};

	static constexpr bool ENABLE_HASH_UPDATE = true;
	static constexpr bool DISABLE_HASH_UPDATE = false;

	template<bool update_hash>
	void place_piece(Piece piece, Square square);

	template<bool update_hash>
	void remove_piece(Square square);

	template<bool update_hash>
	void move_piece(Square from, Square to);

	void reset();

	[[nodiscard]] u8 castling_state(Bitboard from_to) const;

public:
	Position() = default;
	explicit Position(const std::string& fen);

	[[nodiscard]] inline u16 fifty_move_rule() const { return state_history.peek().fifty_move_rule; }
	[[nodiscard]] inline Square ep_square() const { return state_history.peek().ep_square; }
	[[nodiscard]] inline ZobristHash hash() const { return state_history.peek().hash; }
	[[nodiscard]] inline Bitboard from_to() const { return state_history.peek().from_to; }
	[[nodiscard]] inline Color turn() const { return side; }

	enum Repetition : i32 {
		TWO_FOLD,
		THREE_FOLD
	};
	[[nodiscard]] bool has_repetition(Repetition fold = TWO_FOLD);

	template<Color color>
	[[nodiscard]] inline bool king_and_oo_rook_not_moved() const {
		if constexpr (color == WHITE) return !(from_to() & PositionState::WHITE_OO_BANNED_MASK);
		return !(from_to() & PositionState::BLACK_OO_BANNED_MASK);
	}

	template<Color color>
	[[nodiscard]] inline bool king_and_ooo_rook_not_moved() const {
		if constexpr (color == WHITE) return !(from_to() & PositionState::WHITE_OOO_BANNED_MASK);
		return !(from_to() & PositionState::BLACK_OOO_BANNED_MASK);
	}

	[[nodiscard]] inline Piece piece_at(Square square) const { return board[square]; }

	template<Color color, PieceType piece_type>
	[[nodiscard]] constexpr Bitboard occupancy() const { return pieces[make_piece<color, piece_type>()]; }

	template<PieceType piece_type>
	[[nodiscard]] constexpr Bitboard occupancy() const { return occupancy<WHITE, piece_type>() | occupancy<BLACK, piece_type>(); }

	template<Piece piece>
	[[nodiscard]] constexpr Bitboard occupancy() const { return pieces[piece]; }

	template<Color color>
	[[nodiscard]] constexpr Bitboard occupancy() const {
		return 	pieces[make_piece<color, PAWN>()] |
				pieces[make_piece<color, KNIGHT>()] |
				pieces[make_piece<color, BISHOP>()] |
				pieces[make_piece<color, ROOK>()] |
				pieces[make_piece<color, QUEEN>()] |
				pieces[make_piece<color, KING>()];
	}

	inline Bitboard occupancy(Color color) const {
		if (color == WHITE) return occupancy<WHITE>();
		return occupancy<BLACK>();
	}

	inline Bitboard occupancy(Color color, PieceType pt) {
		return pieces[make_piece(color, pt)];
	}

	inline Bitboard occupancy(PieceType pt) {
		return pieces[make_piece(WHITE, pt)] | pieces[make_piece(BLACK, pt)];
	}

	[[nodiscard]] inline Bitboard occupancy() {
		return occupancy<WHITE>() | occupancy<BLACK>();
	}

	template<Color color>
	[[nodiscard]] constexpr Bitboard diagonal_sliders() const {
		return occupancy<color, BISHOP>() | occupancy<color, QUEEN>();
	}

	template<Color color>
	[[nodiscard]] constexpr Bitboard orthogonal_sliders() const {
		return occupancy<color, ROOK>() | occupancy<color, QUEEN>();
	}

	template<Color color>
	[[nodiscard]] constexpr Bitboard attackers_of(Square s, Bitboard occ) const {
		return (tables::attacks<PAWN, ~color>(s) & pieces[make_piece<color, PAWN>()]) |
				(tables::attacks<KNIGHT>(s, occ) & pieces[make_piece<color, KNIGHT>()]) |
				(tables::attacks<BISHOP>(s, occ) & (pieces[make_piece<color, BISHOP>()] | pieces[make_piece<color, QUEEN>()])) |
				(tables::attacks<ROOK>(s, occ) & (pieces[make_piece<color, ROOK>()] | pieces[make_piece<color, QUEEN>()]));
	}

	[[nodiscard]] constexpr Bitboard attackers_of(Square s, Bitboard occ) const {
		return attackers_of<BLACK>(s, occ) | attackers_of<WHITE>(s, occ);
	}

	template<Color C> [[nodiscard]] inline bool in_check() const {
		return attackers_of<~C>(lsb(occupancy<C, KING>()), occupancy<WHITE>() | occupancy<BLACK>());
	}

	void set_fen(const std::string& fen);
	[[nodiscard]] std::string fen() const;
	friend std::ostream& operator<<(std::ostream& os, const Position& p);

	template<Color color>
	void play(Move move);

	template<Color color>
	void undo(Move move);

	template<Color color>
	void play_null();

	template<Color color>
	void undo_null();
};
