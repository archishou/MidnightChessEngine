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
};

TEST_F(MoveSearchFixture, MateInOneTest1){
	Position p;
	const std::string& fen = "2R5/8/8/5K1k/8/3R4/8/8 w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_h8 = Move(c8, h8, QUIET);
	Move mate_h3 = Move(d3, h3, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2);
	EXPECT_TRUE(results.best_move == mate_h3 || results.best_move == mate_h8);
}

TEST_F(MoveSearchFixture, MateInOneTest2){
	Position p;
	const std::string& fen = "5qk1/3R4/6pP/6K1/8/8/1B6/8 w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_h7 = Move(h6, h7, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2);
	EXPECT_TRUE(results.best_move == mate_h7);
}

TEST_F(MoveSearchFixture, MateInOneTest3){
	Position p;
	const std::string& fen = "r3rqkb/pp1b1pnp/2p1p1p1/4P1B1/2B1N1P1/5N1P/PPP2P2/2KR3R w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<WHITE>(p);
	Move mate_move = Move(e4, f6, QUIET);
	ASSERT_TRUE(results.depth_searched >= 2);
	EXPECT_TRUE(results.best_move == mate_move);
}

TEST_F(MoveSearchFixture, MateInTwoTest1){
	Position p;
	const std::string& fen = "3q4/4b3/2p4p/pk1p1B2/N4P2/P1Q3P1/1P5P/7K w - - 0 1";
	Position::set(fen, p);

	BestMoveSearchResults results_m1, results_m2, results_m3;
	results_m1 = best_move<WHITE>(p);
	p.play<WHITE>(results_m1.best_move);
	results_m2 = best_move<BLACK>(p);
	p.play<BLACK>(results_m2.best_move);
	results_m3 = best_move<WHITE>(p);
	Move mate_move_m1 = Move(f5, d3, QUIET);
	Move mate_move_m2 = Move(b5, a4, CAPTURE);
	Move mate_move_m3 = Move(c3, c2, QUIET);
	ASSERT_TRUE(results_m1.depth_searched >= 3);
	ASSERT_TRUE(results_m2.depth_searched >= 3);
	ASSERT_TRUE(results_m3.depth_searched >= 3);
	EXPECT_TRUE(results_m1.best_move == mate_move_m1);
	EXPECT_TRUE(results_m2.best_move == mate_move_m2);
	EXPECT_TRUE(results_m3.best_move == mate_move_m3);
}
