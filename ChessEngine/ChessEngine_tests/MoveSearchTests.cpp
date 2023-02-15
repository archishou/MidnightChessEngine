//
// Created by Archishmaan Peyyety on 1/10/23.
#include "gtest/gtest.h"
#include "move_generation/position.h"
#include "move_search/search.h"
#include "uci_interpreter/parse_uci_move.h"

class MoveSearchFixture : public ::testing::Test {
protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
		initialize_move_sort_tables();
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
	results = best_move<WHITE>(p, {.debug_info = true});
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

TEST_F(MoveSearchFixture, TTTable){
	Position p;
	const std::string& fen = "8/8/8/6P1/1RN2Q2/2k5/5PP1/5K2 w - - 0 1";
	Position::set(fen, p);
	BestMoveSearchResults best_white = best_move<WHITE>(p);
	std::cout << best_white.pv << std::endl;
	std::cout << best_white.depth_searched << std::endl;
}

TEST_F(MoveSearchFixture, SpecialEndgamFEN){
	Position p;
	const std::string& fen = "8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1";
	const BestMoveSearchParameters parameters = {
			.depth = MAX_DEPTH,
			.time_limit = 1000,
			.debug_info = true
	};
	Position::set(fen, p);
	BestMoveSearchResults best_white = best_move<WHITE>(p, parameters);
	std::cout << best_white << std::endl;
}

TEST_F(MoveSearchFixture, DrawTest1){
	std::string uci_moves = "e2e4 c7c5 g1f3 d7d6 d2d4 g8f6 d4c5 f6e4 c5d6 e4d6 c1f4 b8c6 b1c3 d8b6 c3d5 b6a5 d1d2 "
							"a5d2 f4d2 e8d7 d2f4 e7e6 d5c3 f7f6 f4g3 d6f5 e1c1 d7e7 g3c7 e6e5 f1c4 e7e8 c3e4 c8d7 "
							"h1e1 a8c8 e4d6 f5d6 c7d6 c6e7 b2b3 b7b5 c4e2 d7c6 d6a3 e7f5 a3b2 f8b4 c2c3 c6f3 g2f3 "
							"a7a6 c1c2 e8f8 a2a4 f5d4 c2d3 d4e2 c3b4 e2f4 d3d2 f4d5 a4b5 a6b5 e1e4 f8g8 h2h4 d5f4 "
							"d2e3 f4g6 e3d3 f6f5 e4e1 c8e8 h4h5 g6f4 d3c2 e5e4 f3e4 f5e4 e1e4 e8e4 d1d8 g8f7 d8h8 "
							"f4h5 c2d3 e4b4 h8h7 b4h4 b3b4 h5f4 d3e4 h4h7 e4f4 h7h4 f4e5 h4b4 b2d4 f7g8 f2f4 b4b1 "
							"e5f5 b5b4 f5g6 b4b3 d4e5 b1g1 g6f5 g1g2 f5e4 b3b2 e5b2 g2b2 f4f5 g8f7 e4d5 f7f6 d5e4 "
							"f6g5 f5f6 g5f6 e4d5 f6e7 d5d4 e7d7 d4d5 b2d2 d5e4 d7e6 e4e3 d2g2 e3e4 e6f6 e4f4 g2b2 "
							"f4e4 f6e6 e4d4 b2e2 d4d3 e2f2 d3e4 e6e7 e4d4 e7e8 d4d5 f2e2 d5d4 e8d7 d4c4 e2f2 c4d5 "
							"f2f4 d5e5 f4g4 e5d5 d7d8 d5e5 d8e7 e5d5 e7d7 d5e5 d7d8 e5d5 d8c8 d5d6 g4g5 d6e6 c8b7 "
							"e6d6 b7a7 d6d7 g5g6 d7c7 g6b6 c7d8 a7b8 d8d7 b6f6 d7d8 f6f7 d8e8 f7f2 e8e7 b8c7 e7e6 "
							"c7d8 e6d5 f2d2 d5e5 d8e8 e5e4 e8e7 e4f4 d2b2 f4e5 b2b4 e5d5 b4f4 d5e5 f4c4 e5d5 c4a4 "
							"d5e5 a4a2 e5d4 e7d7 d4d5 a2a4 d5e5 a4b4 e5f5 d7e8 f5e5 e8f7 e5d5 f7g8 d5e5 b4c4 e5d5";
							//"c4c1";
	Position p;
	Position::set(INITIAL_BOARD_FEN, p);
	uci_update_position_from_moves(p, uci_moves);
	BestMoveSearchResults results = best_move(p);
	std::cout << p.fen() << std::endl;
	EXPECT_TRUE(p.at(results.best_move.from()) == BLACK_PAWN);
}

TEST_F(MoveSearchFixture, A1){
	std::string uci_moves = "e2e4 e7e6 d2d4 d7d5 b1c3 g8f6 c1g5 f8e7 e4e5 f6d7 h2h4 e8g8 f1d3 c7c5 d1g4 e7g5 h4g5 c5d4 d3h7";
	Position p;
	Position::set(INITIAL_BOARD_FEN, p);
	uci_update_position_from_moves(p, uci_moves);
	std::cout << p.fen() << std::endl;
	const BestMoveSearchParameters parameters = {
			.depth = 15,
			.time_limit = 1000,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	std::cout << p.fen() << std::endl;
	std::cout << results << std::endl;
}

TEST_F(MoveSearchFixture, MateIn5Test1){
	Position p;
	Position::set("4k3/1Q6/8/8/8/8/6K1/8 w - - 6 4", p);
	std::cout << p.fen() << std::endl;
	const BestMoveSearchParameters parameters = {
			.depth = MAX_DEPTH,
			.time_limit = 1000,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	assert(line_size(results.pv) == 9);
}

TEST_F(MoveSearchFixture, MateIn5Test2){
	Position p;
	Position::set("k7/1B6/1NK5/8/8/8/8/8 b - - 0 1", p);
	std::cout << p.fen() << std::endl;
	const BestMoveSearchParameters parameters = {
			.depth = MAX_DEPTH,
			.time_limit = 100000,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	std::cout << results << std::endl;
	//assert(line_size(results.pv) == 9);
}
