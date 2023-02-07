//
// Created by Archishmaan Peyyety on 1/10/23.
//
#include <fstream>
#include "gtest/gtest.h"
#include "move_generation/position.h"
#include "constants.h"
#include "helpers.h"

class MoveGenerationFixture : public ::testing::Test {

protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {}
};

template<Color Us>
unsigned long long perft_node_count(Position& p, unsigned int depth) {
	int nmoves;
	unsigned long long nodes = 0;

	MoveList<Us> list(p);
	if (depth == 1) return (unsigned long long) list.size();
	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft_node_count<~Us>(p, depth - 1);
		p.undo<Us>(move);
	}
	return nodes;
}

unsigned long long test_perft_node_count(const std::string& fen, int depth) {
	Position p;
	Position::set(fen, p);
	if (p.turn() == WHITE) {
		return perft_node_count<WHITE>(p, depth);
	} else {
		return perft_node_count<BLACK>(p, depth);
	}
}

TEST_F(MoveGenerationFixture, PerftDepthOneDefaultFen){
	EXPECT_EQ(test_perft_node_count(INITIAL_BOARD_FEN, 1), 20);
}

TEST_F(MoveGenerationFixture, PerftDepthTwoDefaultFen){
	EXPECT_EQ(test_perft_node_count(INITIAL_BOARD_FEN, 2), 400);
}

TEST_F(MoveGenerationFixture, PerftDepthThreeDefaultFen){
	EXPECT_EQ(test_perft_node_count(INITIAL_BOARD_FEN, 3), 8902);
}

TEST_F(MoveGenerationFixture, PerftDepthFourDefaultFen){
	EXPECT_EQ(test_perft_node_count(INITIAL_BOARD_FEN, 4), 197281);
}

TEST_F(MoveGenerationFixture, PerftDepthFiveDefaultFen){
	EXPECT_EQ(test_perft_node_count(INITIAL_BOARD_FEN, 5), 4865609);
}

TEST_F(MoveGenerationFixture, PerftDepthSixDefaultFen){
	EXPECT_EQ(test_perft_node_count(INITIAL_BOARD_FEN, 6), 119060324);
}

TEST_F(MoveGenerationFixture, PerftDepthOneKiwipete){
	EXPECT_EQ(test_perft_node_count(KIWIPETE_FEN, 1), 48);
}

TEST_F(MoveGenerationFixture, PerftDepthTwoKiwipete){
	EXPECT_EQ(test_perft_node_count(KIWIPETE_FEN, 2), 2039);
}

TEST_F(MoveGenerationFixture, PerftDepthThreeKiwipete){
	EXPECT_EQ(test_perft_node_count(KIWIPETE_FEN, 3), 97862);
}

TEST_F(MoveGenerationFixture, PerftDepthFourKiwipete){
	EXPECT_EQ(test_perft_node_count(KIWIPETE_FEN, 4), 4085603);
}

TEST_F(MoveGenerationFixture, PerftDepthFiveKiwipete){
	EXPECT_EQ(test_perft_node_count(KIWIPETE_FEN, 5), 193690690);
}

TEST_F(MoveGenerationFixture, PerftDepthOneTalkchess){
	EXPECT_EQ(test_perft_node_count(TALKCHESS_FEN, 1), 44);
}

TEST_F(MoveGenerationFixture, PerftDepthTwoTalkchess){
	EXPECT_EQ(test_perft_node_count(TALKCHESS_FEN, 2), 1486);
}

TEST_F(MoveGenerationFixture, PerftDepthThreeTalkchess){
	EXPECT_EQ(test_perft_node_count(TALKCHESS_FEN, 3), 62379);
}

TEST_F(MoveGenerationFixture, PerftDepthFourTalkchess){
	EXPECT_EQ(test_perft_node_count(TALKCHESS_FEN, 4), 2103487);
}

TEST_F(MoveGenerationFixture, PerftDepthFiveTalkchess){
	EXPECT_EQ(test_perft_node_count(TALKCHESS_FEN, 5), 89941194);
}
TEST_F(MoveGenerationFixture, PerftTestPosition){
	EXPECT_EQ(test_perft_node_count("rn2kbnr/pp2pppp/2p5/8/5B2/N2P1B1P/PP3PP1/R4RK1 w kq - 0 13", 5), 18292958);
}

TEST_F(MoveGenerationFixture, Perft126) {
	std::string perft_file_path = "/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/"
								  "ChessEngine_tests/MoveGenerationTests/perft_results.txt";
	std::ifstream input_file(perft_file_path);
	std::string input_line;
	while (std::getline(input_file, input_line)) {
		std::vector<std::string> split_perft = split(input_line, ";");
		std::string fen = split_perft[0];
		for (int i = 1; i < split_perft.size(); i++) {
			std::string expected = split_perft[i];
			int depth = i;
			int expected_node_count = std::stoi(split(expected, " ")[2]);
			std::cout << "Running fen: " << fen << " depth " << depth << " expected node count " << expected_node_count << std::endl;
			EXPECT_EQ(test_perft_node_count(fen, depth), expected_node_count);
			std::cout << "Passed fen: " << fen << " depth " << depth << " expected node count " << expected_node_count << std::endl;
		}
	}
	input_file.close();
}