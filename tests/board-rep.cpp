//
// Created by Archishmaan Peyyety on 1/13/23.
//
#include "lib/doctests.h"
#include "move_search/types.h"

TEST_SUITE_BEGIN("eval");

TEST_CASE("piece-types") {
	CHECK_EQ(PAWN, type_of(WHITE_PAWN));
	CHECK_EQ(KNIGHT, type_of(WHITE_KNIGHT));
	CHECK_EQ(BISHOP, type_of(WHITE_BISHOP));
	CHECK_EQ(ROOK, type_of(WHITE_ROOK));
	CHECK_EQ(QUEEN, type_of(WHITE_QUEEN));
	CHECK_EQ(KING, type_of(WHITE_KING));

	CHECK_EQ(PAWN, type_of(BLACK_PAWN));
	CHECK_EQ(KNIGHT, type_of(BLACK_KNIGHT));
	CHECK_EQ(BISHOP, type_of(BLACK_BISHOP));
	CHECK_EQ(ROOK, type_of(BLACK_ROOK));
	CHECK_EQ(QUEEN, type_of(BLACK_QUEEN));
	CHECK_EQ(KING, type_of(BLACK_KING));

	CHECK_EQ(NO_PIECE_TYPE, type_of(NO_PIECE));
}

TEST_SUITE_END();
