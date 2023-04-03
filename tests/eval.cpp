//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "lib/doctests.h"
#include "move_search/types.h"
#include "engine.h"
#include "evaluation/bitboards.h"

TEST_SUITE_BEGIN("eval");

TEST_CASE("king-ring"){
	initialize_engine();
	Position p;
	Position::set("r3kb2/ppppp1p1/4q2r/5pn1/5n2/4PQ1K/PPPP1PPP/RNB1Nb1R w q - 0 1", p);
	constexpr Color Us = WHITE;
	constexpr Color Them = ~Us;

	Bitboard us_king = p.bitboard_of(Us, KING);
	Bitboard us_pieces = p.all_pieces<Us>();
	Bitboard them_pieces = p.all_pieces<~Us>();
	Square king_square = pop_lsb(&us_king);
	Bitboard king_ring = (KING_ATTACKS[king_square] & ~us_pieces);
	print_bitboard(king_ring);

	while (king_ring) {
		Square king_attack_square = pop_lsb(&king_ring);
		Bitboard them_attack_ring = p.attackers_from<Them>(king_attack_square, us_pieces | them_pieces);
		if (them_attack_ring) print_bitboard(them_attack_ring);
		while (them_attack_ring) {
			Square attacking_piece_square = pop_lsb(&them_attack_ring);
			PieceType attacking_piece = type_of(p.at(attacking_piece_square));
			std::cout << attacking_piece << std::endl;
		}
	}

	Bitboard checkers = p.attackers_from<Them>(king_square, us_pieces | them_pieces);
	if (checkers) print_bitboard(checkers);
	while (checkers) {
		Square checker_square = pop_lsb(&checkers);
		PieceType attacking_piece = type_of(p.at(checker_square));
		std::cout << attacking_piece << std::endl;
	}
}

TEST_CASE("psuedo-legal"){
	initialize_engine();
	Position p;
	Position::set("r3kb2/ppppp1p1/4q2r/5pn1/5n2/4PQ1K/PPPP1PPP/RNB1Nb1R w q - 0 1", p);
	constexpr Color Us = BLACK;
	constexpr Color Them = ~Us;

	const Bitboard them_pieces = p.all_pieces<Them>();
	Bitboard us_pieces = p.all_pieces<Us>();

	const PieceType piece = ROOK;
	Square queen_square = bsf(p.bitboard_of(Us, piece));
	Bitboard pseudo_legal_moves = attacks<piece>(queen_square, them_pieces | us_pieces) & ~us_pieces;

	print_bitboard(pseudo_legal_moves);
}

TEST_CASE("attacked-by-pawn"){
	initialize_engine();
	Position p;
	Position::set("r3kb2/ppppp1p1/4q2r/3P1Pn1/5n2/4PQPK/P2P1P2/RNB1Nb1R w q - 0 1", p);
	constexpr Color Us = BLACK;
	constexpr Color Them = ~Us;

	const Bitboard them_pawns = p.bitboard_of(~Us, PAWN);
	const Bitboard attacking_pawns = them_pawns & pawn_attacks<Us>(e6);

	print_bitboard(attacking_pawns);
}

TEST_SUITE_END();
