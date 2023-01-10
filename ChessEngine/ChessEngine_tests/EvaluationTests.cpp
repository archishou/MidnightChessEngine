//
// Created by Archishmaan Peyyety on 1/10/23.
//
#include "gtest/gtest.h"
#include "move_generation/position.h"
#include "evaluation/evaluate.h"

class EvaluationTestsFixture : public ::testing::Test {
protected:
	virtual void SetUp() {
		initialise_all_databases();
		zobrist::initialise_zobrist_keys();
	}

	virtual void TearDown() {

	}
};

TEST_F(EvaluationTestsFixture, DefaultFenPSTQWhite){
	Position p;
	Position::set(DEFAULT_FEN, p);
	int pstq = evaluate_all_piece_positions<WHITE>(p);
	int expected_pstq = 0;
	// Add Pawns
	for (int i = 48; i <= 55; i++) {
		expected_pstq += pawn_pstq[i];
	}
	expected_pstq += rook_pstq[56] + rook_pstq[63];
	expected_pstq += knight_pstq[57] + knight_pstq[62];
	expected_pstq += bishop_pstq[58] + bishop_pstq[61];
	expected_pstq += queen_psqt[59];
	expected_pstq += king_middle[60];
	EXPECT_EQ(pstq, expected_pstq);
}

TEST_F(EvaluationTestsFixture, DefaultFenPSTQBlack){
	Position p;
	Position::set(DEFAULT_FEN, p);
	int pstq = evaluate_all_piece_positions<BLACK>(p);
	int expected_pstq = 0;
	// Add Pawns
	for (int i = 48; i <= 55; i++) {
		expected_pstq += pawn_pstq[i];
	}
	expected_pstq += rook_pstq[56] + rook_pstq[63];
	expected_pstq += knight_pstq[57] + knight_pstq[62];
	expected_pstq += bishop_pstq[58] + bishop_pstq[61];
	expected_pstq += queen_psqt[59];
	expected_pstq += king_middle[60];
	EXPECT_EQ(pstq, expected_pstq);
}
