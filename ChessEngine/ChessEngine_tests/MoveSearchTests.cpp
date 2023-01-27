//
// Created by Archishmaan Peyyety on 1/10/23.
//
// Created by Archishmaan Peyyety on 1/10/23.
//
#include "gtest/gtest.h"
#include "move_generation/position.h"
#include "move_search/search.h"

class MoveSearchFixture : public ::testing::Test {
protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {

	}
	const std::string kiwipete_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
};

const std::string& ASSERTION_ERR_SEARCH_DEPTH = "Test not completed, did not search deep enough";

bool lines_equal(Line& expected, Line& results_line) {
	for (int i = 0; i < LINE_SIZE; ++i) {
		if (expected[i] != results_line[i]) {
			return false;
		}
	}
	return true;
}

TEST_F(MoveSearchFixture, MateInOneTest1){
	Position p;
	const std::string& fen = "2R5/8/8/5K1k/8/3R4/8/8 w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_h8 = Move(c8, h8, QUIET);
	Move mate_h3 = Move(d3, h3, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2) << ASSERTION_ERR_SEARCH_DEPTH;
	EXPECT_TRUE(results.best_move == mate_h3 || results.best_move == mate_h8);
}

TEST_F(MoveSearchFixture, MateInOneTest2){
	Position p;
	const std::string& fen = "5qk1/3R4/6pP/6K1/8/8/1B6/8 w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_h7 = Move(h6, h7, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2) << ASSERTION_ERR_SEARCH_DEPTH;
	EXPECT_TRUE(results.best_move == mate_h7);
}

TEST_F(MoveSearchFixture, MateInOneTest3){
	Position p;
	const std::string& fen = "r3rqkb/pp1b1pnp/2p1p1p1/4P1B1/2B1N1P1/5N1P/PPP2P2/2KR3R w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_move = Move(e4, f6, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2) << ASSERTION_ERR_SEARCH_DEPTH;
	EXPECT_TRUE(results.best_move == mate_move);
}

TEST_F(MoveSearchFixture, MateInOneTest4){
	Position p;
	const std::string& fen = "7k/6pp/8/8/2R4q/8/8/K7 w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_move = Move(c4, c8, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2);
	EXPECT_TRUE(results.best_move == mate_move);
}

TEST_F(MoveSearchFixture, MateInTwoTest1){
	Position p;
	const std::string& fen = "3q4/4b3/2p4p/pk1p1B2/N4P2/P1Q3P1/1P5P/7K w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	ASSERT_TRUE(results.depth_searched >= 3) << ASSERTION_ERR_SEARCH_DEPTH;
	Line expected_line;
	expected_line[0] = Move(f5, d3, QUIET);
	expected_line[1] = Move(b5, a4, CAPTURE);
	expected_line[2] = Move(c3, c2, QUIET);
	EXPECT_TRUE(lines_equal(expected_line, results.pv));
}

TEST_F(MoveSearchFixture, MateInTwoTest2){
	Position p;
	const std::string& fen = "r3qk1r/p5pp/3pQpb1/2n5/2B2B2/8/P4PPP/4R1K1 w - - 0 1";

	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	ASSERT_TRUE(results.depth_searched >= 3) << ASSERTION_ERR_SEARCH_DEPTH;
	Line expected_line_1;
	expected_line_1[0] = Move(e6, d6, CAPTURE);
	expected_line_1[1] = Move(e8, e7, QUIET);
	expected_line_1[2] = Move(d6, e7, CAPTURE);

	Line expected_line_2;
	expected_line_2[0] = Move(f4, d6, CAPTURE);
	expected_line_2[1] = Move(e8, e7, QUIET);
	expected_line_2[2] = Move(e6, e7, CAPTURE);
	EXPECT_TRUE(lines_equal(expected_line_1, results.pv) || lines_equal(expected_line_2, results.pv));
}

TEST_F(MoveSearchFixture, QSearchTest1){
	Position p;
	const std::string& fen = "3k2r1/6p1/8/7N/3K4/8/8/8 w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchParameters search_parameters{};
	search_parameters.depth = 1;
	search_parameters.time_limit = 1000;
	BestMoveSearchResults results;
	results = best_move<WHITE>(p, search_parameters);
	Move horizon_effected_capture = Move(h5, g7, CAPTURE);
	ASSERT_TRUE(results.depth_searched == 1) << ASSERTION_ERR_SEARCH_DEPTH;
	EXPECT_TRUE(results.best_move != horizon_effected_capture);
}

TEST_F(MoveSearchFixture, ImportantFENToAnalyze){
	Position p;
	const std::string& fen = "8/1k6/p3R3/1p6/1P1p4/P1b1N3/r4PP1/6K1 w - - 0 1";
	Position::set(fen, p);
	std::cout << p << std::endl;
	BestMoveSearchResults mov = best_move<WHITE>(p);
	std::cout << mov.pv << std::endl;
	p.play<WHITE>(Move(e3, d1, QUIET));
	p.play<BLACK>(Move(a2, a1, QUIET));
	BestMoveSearchResults best_white = best_move<WHITE>(p, {4, 1000});

	std::cout << best_white.pv << std::endl;
}