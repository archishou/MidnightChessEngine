#pragma once
#include <cstdint>
#include "../../types.h"
#include "../../board/types/board_types.h"
#include "../../board/types/square.h"

using MoveType = u8;

constexpr MoveType QUIET		= 0b0000;
constexpr MoveType OO			= 0b0001;
constexpr MoveType OOO			= 0b0010;
constexpr MoveType DOUBLE_PUSH	= 0b0011;

// Captures have the 4th bit set.
constexpr MoveType CAPTURE_TYPE	= 0b1000;
constexpr MoveType ENPASSANT	= 0b1001;

// Promotions have the 3rd bit set.
constexpr MoveType PROMOTION_TYPE 	= 0b0100;
constexpr MoveType PR_KNIGHT		= 0b0100;
constexpr MoveType PR_BISHOP		= 0b0101;
constexpr MoveType PR_ROOK			= 0b0110;
constexpr MoveType PR_QUEEN			= 0b0111;

class Move {
private:
	// Bits are arranged as follows
	// | 4 bits for type | 6 bits for from | 6 bits for to
	uint16_t move;

	static constexpr u8 TO_SHIFT	= 0;
	static constexpr u8 FROM_SHIFT	= 6;
	static constexpr u8 TYPE_SHIFT	= 12;

	static constexpr u8 TO_BITMASK		= 0b111111;
	static constexpr u8 FROM_BITMASK	= 0b111111;
	static constexpr u8 TYPE_BITMASK	= 0b1111;

	static constexpr u8 CAPTURE_BITMASK		= 0b1000;
	static constexpr u8 PROMOTION_BITMASK	= 0b0100;

public:
	constexpr Move() : move(0) {}

	constexpr explicit Move(uint16_t m) { move = m; }

	constexpr Move(Square from, Square to) : move(0) {
		move = (from << 6) | to;
	}

	constexpr Move(Square from, Square to, MoveType type) : move(0) {
		move = (type << TYPE_SHIFT) | (from << FROM_SHIFT) | to << TO_SHIFT;
	}

	explicit Move(const std::string& m) {
		move = (create_square(File(m[0] - 'a'), Rank(m[1] - '1')) << 6) |
				create_square(File(m[2] - 'a'), Rank(m[3] - '1'));
	}

	[[nodiscard]] inline Square to() const { return Square(move & TO_BITMASK); }
	[[nodiscard]] inline Square from() const { return Square((move >> FROM_SHIFT) & FROM_BITMASK); }
	[[nodiscard]] inline MoveType type() const { return (move >> TYPE_SHIFT) & TYPE_BITMASK; }

	[[nodiscard]] inline bool is_capture() const { return (move >> TYPE_SHIFT) & CAPTURE_BITMASK; }
	[[nodiscard]] inline bool is_promotion() const { return (move >> TYPE_SHIFT) & PROMOTION_BITMASK; }
	[[nodiscard]] inline bool is_quiet() const { return !is_capture() && !is_promotion(); }

	bool operator==(Move a) const { return move == a.move; }
	bool operator!=(Move a) const { return move != a.move; }
};

constexpr Move EMPTY_MOVE = Move();

inline array<string , 16> MOVE_TYPE_UCI = {
		"", "", "", "", "N", "B", "R", "Q",
		"", "", "", "", "N", "B", "R", "Q"
};

inline std::ostream& operator<<(std::ostream& os, const Move& m) {
	os << SQ_TO_STRING[m.from()] << SQ_TO_STRING[m.to()] << MOVE_TYPE_UCI[m.type()];
	return os;
}
