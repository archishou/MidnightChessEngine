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
		t_table.reset_table();
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


TEST_F(MoveSearchFixture, IllegalMoveA1a1){
	std::string uci_moves;
	uci_moves = "d2d4 d7d6 e2e4 g8f6 b1c3 b8d7 g2g4 g7g6 g4g5 f6h5 g1f3 e7e5 d4e5 d6e5 f1c4 f8g7 e1g1 e8g8 c3d5 d7c5 "
				"d1e2 c7c6 d5b4 c8e6 c4e6 c5e6 b4d3 d8c7 c1d2 c6c5 d2c3 h5f4 e2e3 e6d4 d3f4 e5f4 e3d3 a8d8 f3d4 c5d4 "
				"c3b4 f8e8 f1e1 c7d7 g1h1 d7g4 e1g1 g4e6 g1e1 g7e5 b4a5 d8d7 h2h3 e6e7 e1g1 e5g7 a1e1 e7e6 a2a3 g7e5 "
				"g1g4 e8c8 h1g1 e6c4 d3c4 c8c4 c2c3 d4d3 a5b4 d3d2 e1d1 c4e4 g1f1 d7d5 b4e7 e5c3 b2c3 e4e7 g4f4 e7c7 "
				"c3c4 d5d3 f1g2 c7c5 h3h4 b7b5 c4b5 c5c1 d1d2 d3d2 f4a4 c1c2 a4f4 c2b2 a3a4 d2c2 g2g1 b2a2 g1g2 c2c1 "
				"f4d4 c1a1 d4d8 g8g7 g2g3 a2a4 d8d7 a1a3 f2f3 a4b4 d7b7 a3a5 b7b8 b4b5 b8b5 a5b5 f3f4 b5b3 g3g4 h7h5";
	Position p;
	Position::set(INITIAL_BOARD_FEN, p);
	uci_update_position_from_moves(p, uci_moves);
	BestMoveSearchParameters parameters = {
			.depth = MAX_DEPTH,
			.time_limit = 100,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	EXPECT_TRUE(line_size(results.pv) >= 1);
}

TEST_F(MoveSearchFixture, IllegalMoveA1a12){
	std::string uci_moves;
	uci_moves = "d2d4 d7d5 c2c4 e7e6 b1c3 c7c6 e2e4 d5e4 c3e4 f8b4 e4c3 c6c5 a2a3 b4c3 b2c3 g8f6 g1f3 b8c6 f1e2 e8g8 "
				"e1g1 d8c7 c1e3 c5d4 c3d4 f6g4 d1b3 c6a5 b3c2 g4e3 f2e3 a5c6 e2d3 f7f5 e3e4 f8d8 e4f5 c6d4 f3d4 c7c5 "
				"f1f4 e6e5 f4h4 d8d4 c2f2 c5b6 h4d4 b6d4 f2d4 e5d4 a1f1 c8d7 f1f4 a8f8 g2g4 f8e8 f4d4 d7c6 c4c5 e8e1 "
				"g1f2 e1h1 d3c4 g8f8 f2g3 h1c1 h2h3 f8e7 d4f4 h7h6 f4d4 e7f8 h3h4 c1c3 g3h2 f8e7 d4f4 e7f6 c4e6 c3c2 "
				"h2h3 c2c5 f4c4 c5e5 c4c3 f6e7 e6c4 h6h5 c3g3 e7f8 c4e6 h5g4 g3g4 e5e1 g4g3 e1h1 h3g4 h1a1 g4h3 a1f1 "
				"g3b3 c6e4 b3b4 f1e1 b4a4 a7a6 a4d4 e4c6 d4d8 f8e7 d8g8 e1g1 g8c8 g1a1 c8g8 a1a3 h3g4 e7f6 g8f8 f6e5 "
				"h4h5 c6f3 g4g5 a3a1 e6c8 a1g1 g5h4 e5f4 f5f6 g1h1 c8h3 g7g5";
	Position p;
	Position::set(INITIAL_BOARD_FEN, p);
	uci_update_position_from_moves(p, uci_moves);
	BestMoveSearchParameters parameters = {
			.depth = MAX_DEPTH,
			.time_limit = 100,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	EXPECT_TRUE(line_size(results.pv) >= 1);
}

TEST_F(MoveSearchFixture, A1){
	std::string uci_moves = "e2e4 e7e6 d2d4 d7d5 b1c3 g8f6 c1g5 f8e7 e4e5 f6d7 h2h4 e8g8 f1d3 c7c5 d1g4 e7g5 h4g5 c5d4 d3h7";
	Position p;
	Position::set(INITIAL_BOARD_FEN, p);
	uci_update_position_from_moves(p, uci_moves);
	std::cout << p.fen() << std::endl;
	const BestMoveSearchParameters parameters = {
			.depth = 15,
			.time_limit = 100,
			.debug_info = false
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
			.time_limit = 100000,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	EXPECT_EQ(line_size(results.pv), 9);
}

TEST_F(MoveSearchFixture, MateIn6Test1){
	Position p;
	Position::set("k7/1B6/1NK5/8/8/8/8/8 b - - 0 1", p);
	std::cout << p.fen() << std::endl;
	const BestMoveSearchParameters parameters = {
			.depth = MAX_DEPTH,
			.time_limit = 1000,
			.debug_info = true
	};
	BestMoveSearchResults results = best_move(p, parameters);
	EXPECT_EQ(line_size(results.pv), 12);
}

TEST_F(MoveSearchFixture, CountIsolatedPawns1) {
	Position p;
	Position::set("5q2/6k1/2b5/1pB5/1n2QpB1/r3b2p/3PK1pN/8 w - - 0 1", p);
	int num_black, num_white;
	num_black = pop_count(isolated_pawns<BLACK>(p));
	num_white = pop_count(isolated_pawns<WHITE>(p));
	EXPECT_EQ(num_black, 1);
	EXPECT_EQ(num_white, 1);
}


TEST_F(MoveSearchFixture, CountIsolatedPawns2) {
	Position p;
	Position::set("1R1Bbn2/1pP5/3b1KN1/n3P3/8/1B2Q1pr/6k1/8 w - - 0 1", p);
	int num_black, num_white;
	num_black = pop_count(isolated_pawns<BLACK>(p));
	num_white = pop_count(isolated_pawns<WHITE>(p));
	EXPECT_EQ(num_black, 2);
	EXPECT_EQ(num_white, 2);
}

TEST_F(MoveSearchFixture, CountIsolatedPawns3) {
	Position p;
	Position::set("2N3N1/R5n1/1pp4n/4r3/4B3/P5K1/kbP1p3/7b w - - 0 1", p);
	int num_black, num_white;
	num_black = pop_count(isolated_pawns<BLACK>(p));
	num_white = pop_count(isolated_pawns<WHITE>(p));
	EXPECT_EQ(num_black, 1);
	EXPECT_EQ(num_white, 2);
}

TEST_F(MoveSearchFixture, CountIsolatedPawns4) {
	Position p;
	Position::set("1q6/1P4b1/pn1P4/4k3/N2r1p2/4PR2/1K5N/3r3b w - - 0 1", p);
	int num_black, num_white;
	num_black = pop_count(isolated_pawns<BLACK>(p));
	num_white = pop_count(isolated_pawns<WHITE>(p));
	EXPECT_EQ(num_black, 2);
	EXPECT_EQ(num_white, 1);
}

TEST_F(MoveSearchFixture, CountIsolatedPawns5) {
	Position p;
	Position::set("1k2N3/5rP1/1r2R2p/5Q2/8/2PN1P1R/4n1np/5K2 w - - 0 1", p);
	int num_black, num_white;
	num_black = pop_count(isolated_pawns<BLACK>(p));
	num_white = pop_count(isolated_pawns<WHITE>(p));
	EXPECT_EQ(num_black, 2);
	EXPECT_EQ(num_white, 1);
}

TEST_F(MoveSearchFixture, OpenFiles) {
	Position p;
	Position::set("r2qk1nr/1p3p1p/p1p1p3/1b1p1p2/1PP1b3/P3P3/3P1PPP/RNBQKBNR w KQkq - 0 1", p);
}

TEST_F(MoveSearchFixture, PositionInCheck) {
	Position p;
	Position::set("rnbqkbnr/ppppp1pp/8/5p1Q/4P3/8/PPPP1PPP/RNB1KBNR b KQkq - 0 1", p);
	EXPECT_TRUE(p.in_check<BLACK>());
	EXPECT_FALSE(p.in_check<WHITE>());

	Position::set("rnb1kbnr/ppppp2p/6p1/5p1Q/4P3/4q3/PPPP1PPP/RNB1KBNR w KQkq - 0 1", p);
	EXPECT_TRUE(p.in_check<WHITE>());
	EXPECT_FALSE(p.in_check<BLACK>());

	Position::set("rnb1kbnr/ppppp2p/6p1/5p1Q/4P3/4q3/PPPPBPPP/RNB1K1NR b KQkq - 0 1", p);
	EXPECT_FALSE(p.in_check<WHITE>());
	EXPECT_FALSE(p.in_check<BLACK>());
}
