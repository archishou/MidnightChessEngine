//
// Created by Archishmaan Peyyety on 5/2/23.
//

#include "lib/doctests.h"
#include "iostream"
#include "fstream"
#include "../src/utils/helpers.h"
#include "../src/move_gen/tables/attack_tables.h"

TEST_SUITE_BEGIN("attacks");

TEST_CASE("attacks-10000") {
	std::string attacks_file_path = "./tests/attacks.txt";
	std::ifstream input_file(attacks_file_path);
	std::string input_line;
	while (std::getline(input_file, input_line)) {
		std::vector<string> tokens = split(input_line, ",");
		Bitboard occupancy = std::stoull(tokens[0]);
		auto square = static_cast<Square>(std::stoi(tokens[1]));
		Bitboard expected_white_pawn_attacks = std::stoull(tokens[2]);
		Bitboard expected_black_pawn_attacks = std::stoull(tokens[3]);
		Bitboard expected_knight_pawn_attacks = std::stoull(tokens[4]);
		Bitboard expected_bishop_attacks = std::stoull(tokens[5]);
		Bitboard expected_rook_attacks = std::stoull(tokens[6]);
		Bitboard expected_queen_attacks = std::stoull(tokens[7]);
		Bitboard expected_king_attacks = std::stoull(tokens[8]);

		CHECK_EQ(tables::attacks<PAWN, WHITE>(square), expected_white_pawn_attacks);
		CHECK_EQ(tables::attacks<PAWN, BLACK>(square), expected_black_pawn_attacks);
		CHECK_EQ(tables::attacks<KNIGHT>(square, occupancy), expected_knight_pawn_attacks);
		CHECK_EQ(tables::attacks<BISHOP>(square, occupancy), expected_bishop_attacks);
		CHECK_EQ(tables::attacks<ROOK>(square, occupancy), expected_rook_attacks);
		CHECK_EQ(tables::attacks<QUEEN>(square, occupancy), expected_queen_attacks);
		CHECK_EQ(tables::attacks<KING>(square, occupancy), expected_king_attacks);
	}
}

TEST_SUITE_END();