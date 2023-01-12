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
	// Different from position.h/Square, 0th square is a8. Used for PSTQ testing.
	// Black squares are manually reflected across mid-line. Should be automatically done in PSTQ method.
	enum AbsoluteBoardSquare : int {
		a8, b8, c8, d8, e8, f8, g8, h8,
		a7, b7, c7, d7, e7, f7, g7, h7,
		a6, b6, c6, d6, e6, f6, g6, h6,
		a5, b5, c5, d5, e5, f5, g5, h5,
		a4, b4, c4, d4, e4, f4, g4, h4,
		a3, b3, c3, d3, e3, f3, g3, h3,
		a2, b2, c2, d2, e2, f2, g2, h2,
		a1, b1, c1, d1, e1, f1, g1, h1,
		NO_SQUARE
	};

};

TEST_F(EvaluationTestsFixture, DefaultFenPSTQWhite){
	Position p;
	Position::set(DEFAULT_FEN, p);
	int pstq = evaluate_all_piece_positions<WHITE>(p);
	int expected_pstq = 0;
	// Add Pawns
	for (int i = a2; i <= h2; i++) {
		expected_pstq += pawn_pstq[i];
	}
	expected_pstq += rook_pstq[a1] + rook_pstq[h1];
	expected_pstq += knight_pstq[b1] + knight_pstq[g1];
	expected_pstq += bishop_pstq[c1] + bishop_pstq[f1];
	expected_pstq += queen_psqt[d1];
	expected_pstq += king_middle[e1];
	EXPECT_EQ(pstq, expected_pstq);
}

TEST_F(EvaluationTestsFixture, DefaultFenPSTQBlack){
	Position p;
	Position::set(DEFAULT_FEN, p);
	int pstq = evaluate_all_piece_positions<BLACK>(p);
	int expected_pstq = 0;
	// Add Pawns
	for (int i = a2; i <= h2; i++) {
		expected_pstq += pawn_pstq[i];
	}
	expected_pstq += rook_pstq[a1] + rook_pstq[h1];
	expected_pstq += knight_pstq[b1] + knight_pstq[g1];
	expected_pstq += bishop_pstq[c1] + bishop_pstq[f1];
	expected_pstq += queen_psqt[d1];
	expected_pstq += king_middle[e1];
	EXPECT_EQ(pstq, expected_pstq);
}

TEST_F(EvaluationTestsFixture, KingEndgamePSTQ){
	Position p;
	Position::set("8/5p2/pb3p1K/8/8/8/6q1/1k6 w - - 17 64", p);

	int pstq = evaluate_all_piece_positions<BLACK>(p);
	int expected_pstq = 0;
	// Add Pawns
	expected_pstq += pawn_pstq[a3] + pawn_pstq[f3] + pawn_pstq[f2];
	expected_pstq += bishop_pstq[b3];
	expected_pstq += queen_psqt[g7];
	expected_pstq += king_middle[b8];
	EXPECT_EQ(pstq, expected_pstq);
}
