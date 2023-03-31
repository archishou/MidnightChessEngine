//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "lib/doctests.h"
#include "move_search/types.h"
#include "move_search/move_ordering/move_ordering.h"
#include "engine.h"

TEST_SUITE_BEGIN("eval");

TEST_CASE("king-ring"){
	initialize_engine();
	Position p;
	Position::set("r3kb2/ppppppp1/4q2r/6n1/5n2/4P2K/PPPP1PPP/RNBQNb1R w q - 0 1", p);
	constexpr Color Us = WHITE;
	constexpr Color Them = ~Us;

	Bitboard us_king = p.bitboard_of(Us, KING);
	Bitboard us_pieces = p.all_pieces<Us>();
	Square king_square = pop_lsb(&us_king);
	Bitboard king_ring = (KING_ATTACKS[king_square] & ~us_pieces);
	print_bitboard(king_ring);

	while (king_ring) {
		Square king_attack_square = pop_lsb(&king_ring);
		Bitboard them_attack_ring = p.attackers_from<Them>(king_attack_square, us_pieces);
		while (them_attack_ring) {
			Square attacking_piece_square = pop_lsb(&them_attack_ring);
			PieceType attacking_piece = type_of(p.at(attacking_piece_square));
			std::cout << attacking_piece << std::endl;
		}
	}

	Bitboard checkers = p.attackers_from<Them>(king_square, us_pieces);
	while (checkers) {
		Square checker_square = pop_lsb(&checkers);
		PieceType attacking_piece = type_of(p.at(checker_square));
		std::cout << attacking_piece << std::endl;
	}
}

TEST_SUITE_END();
