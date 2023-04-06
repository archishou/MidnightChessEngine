//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "lib/doctests.h"
#include "move_search/types.h"
#include "move_search/move_ordering/move_ordering.h"

TEST_SUITE_BEGIN("move-ordering");

TEST_CASE("sorting-test-one"){
	ScoredMoves scored_moves;
	ScoredMove m1 = {
			.move = Move(a1, a1),
			.score = 10000,
	};
	ScoredMove m2 = {
			.move = Move(a2, a2),
			.score = 1000,
	};
	ScoredMove m3 = {
			.move = Move(a3, a3),
			.score = 100,
	};
	ScoredMove m4 = {
			.move = Move(a4, a4),
			.score = 10,
	};
	ScoredMove m5 = {
			.move = Move(a5, a5),
			.score = 10,
	};
	ScoredMove m6 = {
			.move = Move(a6, a6),
			.score = 1,
	};
	scored_moves.push_back(m1);
	scored_moves.push_back(m2);
	scored_moves.push_back(m3);
	scored_moves.push_back(m4);
	scored_moves.push_back(m5);
	scored_moves.push_back(m6);
	CHECK_EQ(select_move(scored_moves, 0), Move(a6, a6));
	CHECK_EQ(select_move(scored_moves, 1), Move(a4, a4));
	CHECK_EQ(select_move(scored_moves, 2), Move(a5, a5));
	CHECK_EQ(select_move(scored_moves, 3), Move(a3, a3));
	CHECK_EQ(select_move(scored_moves, 4), Move(a2, a2));
	CHECK_EQ(select_move(scored_moves, 5), Move(a1, a1));
}
TEST_SUITE_END();
