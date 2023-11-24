#include "../types/types.h"
#include "../../board/types/bitboard.h"
#include "../../board/types/piece.h"
#include "../../board/constants/zobrist_constants.h"
#include "attack_tables.h"

namespace tables {
	static array<array<Bitboard, NSQUARES>, NSQUARES> generate_squares_in_between() {
		array<array<Bitboard, NSQUARES>, NSQUARES> squares_in_between{};
		for (Square sq1 = a1; sq1 < NSQUARES; sq1++) {
			for (Square sq2 = a1; sq2 < NSQUARES; sq2++) {
				Bitboard sqs = square_to_bitboard(sq1) | square_to_bitboard(sq2);

				if (file_of(sq1) == file_of(sq2) || rank_of(sq1) == rank_of(sq2))
					squares_in_between[sq1][sq2] = generate_slow_rook_attacks(sq1, sqs) & generate_slow_rook_attacks(sq2, sqs);

				else if (diagonal_of(sq1) == diagonal_of(sq2) || anti_diagonal_of(sq1) == anti_diagonal_of(sq2))
					squares_in_between[sq1][sq2] = generate_slow_bishop_attacks(sq1, sqs) & generate_slow_bishop_attacks(sq2, sqs);
			}
		}
		return squares_in_between;
	}
	static array<array<Bitboard, NSQUARES>, NSQUARES> squares_in_between = generate_squares_in_between();

	static array<array<Bitboard, NSQUARES>, NSQUARES> generate_square_line() {
		array<array<Bitboard, NSQUARES>, NSQUARES> square_line{};
		for (Square sq1 = a1; sq1 < NSQUARES; sq1++) {
			for (Square sq2 = a1; sq2 < NSQUARES; sq2++) {

				if (file_of(sq1) == file_of(sq2) || rank_of(sq1) == rank_of(sq2))
					square_line[sq1][sq2] = generate_slow_rook_attacks(sq1, 0) & generate_slow_rook_attacks(sq2, 0);

				else if (diagonal_of(sq1) == diagonal_of(sq2) || anti_diagonal_of(sq1) == anti_diagonal_of(sq2))
					square_line[sq1][sq2] = generate_slow_bishop_attacks(sq1, 0) & generate_slow_bishop_attacks(sq2, 0);
			}
		}
		return square_line;
	}
	static array<array<Bitboard, NSQUARES>, NSQUARES> square_line = generate_square_line();

	inline static Bitboard square_in_between(Square sq1, Square sq2) {
		return squares_in_between[sq1][sq2];
	}

	inline static Bitboard line_of(Square sq1, Square sq2) {
		return square_line[sq1][sq2];
	}
}