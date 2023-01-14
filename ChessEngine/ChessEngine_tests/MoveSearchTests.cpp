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

TEST_F(MoveSearchFixture, TestFen1){
	Position p;
	const std::string& fen = "r1b1k1nr/pppp1pp1/2n4p/4p3/2B1P3/2PP1N2/PP1K1BqP/RN1Q3R b kq - 0 1";
	Position::set(fen, p);
	BestMoveSearchResults results;
	results = best_move<BLACK>(p);
	// Should be g2f2
	std::cout << results.best_move << std::endl;
	std::cout << results.time_searched << std::endl;
	std::cout << results.depth_searched << std::endl;

	EXPECT_EQ(1, 1);
}

TEST_F(MoveSearchFixture, TestFen3){
	Position p;
	const std::string& fen = "r1bqkb1r/pp1ppppp/2n2n2/2p5/2B1P3/3P1N2/PPP2PPP/RNBQK2R b KQkq - 0 1";
	Position::set(fen, p);
	BestMoveSearchResults results;
	results = best_move<BLACK>(p);
	// Should be g2f2
	std::cout << results.best_move << std::endl;
	std::cout << results.time_searched << std::endl;
	std::cout << results.depth_searched << std::endl;
	std::cout << results.value << std::endl;

	EXPECT_EQ(1, 1);
}

TEST_F(MoveSearchFixture, TestFen2){
	Position p;
	const std::string& fen = "8/5p2/pb3p1K/8/8/8/6q1/1k6 w - - 17 64";
	Position::set(fen, p);
	BestMoveSearchResults results;
	results = best_move<BLACK>(p);
	// Should be g2f2
	std::cout << results.best_move << std::endl;
	std::cout << results.time_searched << std::endl;
	std::cout << results.depth_searched << std::endl;
	std::cout << results.value << std::endl;

	EXPECT_EQ(1, 1);
}

TEST_F(MoveSearchFixture, PawnMove){
	Position p;
	const std::string& fen = "rn2kbnr/pp2pppp/2p5/8/5B2/N2P1B1P/PP3PP1/R4RK1 b kq - 0 13";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<BLACK>(p);
	std::cout << results.best_move << std::endl;
	std::cout << results.depth_searched << std::endl;
	std::cout << results.time_searched << std::endl;

	EXPECT_EQ(1, 1);
}

TEST_F(MoveSearchFixture, QueenMove){
	Position p;
	const std::string& fen = "r3kbnr/pp6/3p1p1p/2p1pq2/2P3P1/2B2P2/PPP5/R2Q1RK1 b kq - 0 18";
	Position::set(fen, p);

	BestMoveSearchResults results;
	results = best_move<BLACK>(p);
	std::cout << "Best Move: " << results.best_move << std::endl;
	std::cout << results.depth_searched << std::endl;
	std::cout << results.time_searched << std::endl;

	EXPECT_EQ(1, 1);
}
