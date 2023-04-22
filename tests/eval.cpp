//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "lib/doctests.h"
#include "move_search/types.h"
#include "evaluation/bitboards.h"

TEST_SUITE_BEGIN("eval");

TEST_CASE("passed-pawns") {
	Position p;
	SUBCASE("candidate-passed-pawn-in-passed-pawns") {
		Position::set("rnbqkbnr/ppp3pp/8/3p1p2/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(0, passed_pawns<WHITE>(p));
	}
	SUBCASE("passed-pawn-white-1") {
		Position::set("rnbqkbnr/ppp3pp/8/2p3p1/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(268435456ULL, passed_pawns<WHITE>(p));
	}
	SUBCASE("passed-pawn-white-2") {
		Position::set("rnbqkbnr/6pp/8/1p4p1/1P2P2p/8/P1PP1PPP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(268437504ULL, passed_pawns<WHITE>(p));
	}
	SUBCASE("passed-pawn-white-3") {
		Position::set("rnbqkbnr/7p/3P1p2/1p6/1PP1PP2/2p5/P5PP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(8796093022208ULL, passed_pawns<WHITE>(p));
	}
	SUBCASE("candidate-passed-pawns-1") {
		Position::set("rnbqkbnr/ppp3pp/8/3p1p2/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(268435456ULL, candidate_pawns<WHITE>(p, passed_pawns<WHITE>(p)));
	}
	SUBCASE("candidate-passed-pawns-2") {
		Position::set("rnbqkbnr/ppp4p/8/3P1pp1/4P3/8/PPP2PPP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(34628173824ULL, candidate_pawns<WHITE>(p, passed_pawns<WHITE>(p)));
	}
	SUBCASE("candidate-passed-pawns-2") {
		Position::set("rnbqkbnr/7p/p7/1p1P1pp1/2P1P3/8/PP3PPP/RNBQKBNR w KQkq - 0 1", p);
		CHECK_EQ(335544320ULL, candidate_pawns<WHITE>(p, passed_pawns<WHITE>(p)));
	}
}

TEST_SUITE_END();
