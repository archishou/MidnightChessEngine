#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "lib/doctests.h"
#include "../src/board/position.h"

TEST_SUITE_BEGIN("board-rep");

TEST_CASE("play-undo-white-ep") {
	Position p(START_FEN);

	CHECK_EQ(p.fen(), START_FEN);

	Move move_1 = Move(e2, e4, DOUBLE_PUSH);
	std::string fen_1 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";

	Move move_2 = Move(h7, h6, QUIET);
	std::string fen_2 = "rnbqkbnr/ppppppp1/7p/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1";

	Move move_3 = Move(e4, e5, QUIET);
	std::string fen_3 = "rnbqkbnr/ppppppp1/7p/4P3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1";

	Move move_4 = Move(d7, d5, DOUBLE_PUSH);
	std::string fen_4 = "rnbqkbnr/ppp1ppp1/7p/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1";

	Move move_5 = Move(e5, d6, ENPASSANT);
	std::string fen_5 = "rnbqkbnr/ppp1ppp1/3P3p/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1";

	p.play<WHITE>(move_1);
	CHECK_EQ(p.fen(), fen_1);

	p.play<BLACK>(move_2);
	CHECK_EQ(p.fen(), fen_2);

	p.play<WHITE>(move_3);
	CHECK_EQ(p.fen(), fen_3);

	p.play<BLACK>(move_4);
	CHECK_EQ(p.fen(), fen_4);

	p.play<WHITE>(move_5);
	CHECK_EQ(p.fen(), fen_5);
	p.undo<WHITE>(move_5);

	CHECK_EQ(p.fen(), fen_4);
	p.undo<BLACK>(move_4);

	CHECK_EQ(p.fen(), fen_3);
	p.undo<WHITE>(move_3);

	CHECK_EQ(p.fen(), fen_2);
	p.undo<BLACK>(move_2);

	CHECK_EQ(p.fen(), fen_1);
	p.undo<WHITE>(move_1);

	CHECK_EQ(p.fen(), START_FEN);
}

TEST_CASE("play-undo-black-ep") {
	Position p(START_FEN);

	CHECK_EQ(p.fen(), START_FEN);

	Move m1 = Move(h2, h3, QUIET);
	string m1_fen = "rnbqkbnr/pppppppp/8/8/8/7P/PPPPPPP1/RNBQKBNR b KQkq - 0 1";

	Move m2 = Move(d7, d5, DOUBLE_PUSH);
	string m2_fen = "rnbqkbnr/ppp1pppp/8/3p4/8/7P/PPPPPPP1/RNBQKBNR w KQkq d6 0 1";

	Move m3 = Move(g2, g3, QUIET);
	string m3_fen = "rnbqkbnr/ppp1pppp/8/3p4/8/6PP/PPPPPP2/RNBQKBNR b KQkq - 0 1";

	Move m4 = Move(d5, d4, QUIET);
	string m4_fen = "rnbqkbnr/ppp1pppp/8/8/3p4/6PP/PPPPPP2/RNBQKBNR w KQkq - 0 1";

	Move m5 = Move(e2, e4, DOUBLE_PUSH);
	string m5_fen = "rnbqkbnr/ppp1pppp/8/8/3pP3/6PP/PPPP1P2/RNBQKBNR b KQkq e3 0 1";

	Move m6 = Move(d4, e3, ENPASSANT);
	string m6_fen = "rnbqkbnr/ppp1pppp/8/8/8/4p1PP/PPPP1P2/RNBQKBNR w KQkq - 0 1";

	p.play<WHITE>(m1);
	CHECK_EQ(p.fen(), m1_fen);

	p.play<BLACK>(m2);
	CHECK_EQ(p.fen(), m2_fen);

	p.play<WHITE>(m3);
	CHECK_EQ(p.fen(), m3_fen);

	p.play<BLACK>(m4);
	CHECK_EQ(p.fen(), m4_fen);

	p.play<WHITE>(m5);
	CHECK_EQ(p.fen(), m5_fen);

	p.play<BLACK>(m6);
	CHECK_EQ(p.fen(), m6_fen);

	p.undo<BLACK>(m6);

	CHECK_EQ(p.fen(), m5_fen);
	p.undo<WHITE>(m5);

	CHECK_EQ(p.fen(), m4_fen);
	p.undo<BLACK>(m4);

	CHECK_EQ(p.fen(), m3_fen);
	p.undo<WHITE>(m3);

	CHECK_EQ(p.fen(), m2_fen);
	p.undo<BLACK>(m2);

	CHECK_EQ(p.fen(), m1_fen);
	p.undo<WHITE>(m1);

	CHECK_EQ(p.fen(), START_FEN);
}

TEST_CASE("play-undo-short-castle") {
	Position p(START_FEN);

	Move m1 = Move(e2, e4, DOUBLE_PUSH);
	std::string m1_fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";

	Move m2 = Move(e7, e5, DOUBLE_PUSH);
	std::string m2_fen = "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 1";

	Move m3 = Move(f1, c4, QUIET);
	std::string m3_fen = "rnbqkbnr/pppp1ppp/8/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR b KQkq - 0 1";

	Move m4 = Move(f8, c5, QUIET);
	std::string m4_fen = "rnbqk1nr/pppp1ppp/8/2b1p3/2B1P3/8/PPPP1PPP/RNBQK1NR w KQkq - 0 1";

	Move m5 = Move(g1, f3, QUIET);
	std::string m5_fen = "rnbqk1nr/pppp1ppp/8/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";

	Move m6 = Move(g8, f6, QUIET);
	std::string m6_fen = "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1";

	Move m7 = Move(e1, g1, OO);
	std::string m7_fen = "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 0 1";

	Move m8 = Move(e8, g8, OO);
	std::string m8_fen = "rnbq1rk1/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 w - - 0 1";

	p.play<WHITE>(m1);
	CHECK_EQ(p.fen(), m1_fen);

	p.play<BLACK>(m2);
	CHECK_EQ(p.fen(), m2_fen);

	p.play<WHITE>(m3);
	CHECK_EQ(p.fen(), m3_fen);

	p.play<BLACK>(m4);
	CHECK_EQ(p.fen(), m4_fen);

	p.play<WHITE>(m5);
	CHECK_EQ(p.fen(), m5_fen);

	p.play<BLACK>(m6);
	CHECK_EQ(p.fen(), m6_fen);

	p.play<WHITE>(m7);
	CHECK_EQ(p.fen(), m7_fen);

	p.play<BLACK>(m8);
	CHECK_EQ(p.fen(), m8_fen);
	p.undo<BLACK>(m8);

	CHECK_EQ(p.fen(), m7_fen);
	p.undo<WHITE>(m7);

	CHECK_EQ(p.fen(), m6_fen);
	p.undo<BLACK>(m6);

	CHECK_EQ(p.fen(), m5_fen);
	p.undo<WHITE>(m5);

	CHECK_EQ(p.fen(), m4_fen);
	p.undo<BLACK>(m4);

	CHECK_EQ(p.fen(), m3_fen);
	p.undo<WHITE>(m3);

	CHECK_EQ(p.fen(), m2_fen);
	p.undo<BLACK>(m2);

	CHECK_EQ(p.fen(), m1_fen);
	p.undo<WHITE>(m1);

	CHECK_EQ(p.fen(), START_FEN);
}

TEST_CASE("play-undo-long-castle") {
	string start_fen = "r3kbnr/ppp1pppp/1nbq4/8/3pP3/1NBQ2PP/PPPP1P2/R3KBNR w KQkq - 0 1";
	Position p(start_fen);

	Move m1 = Move(e1, c1, OOO);
	string m1_fen = "r3kbnr/ppp1pppp/1nbq4/8/3pP3/1NBQ2PP/PPPP1P2/2KR1BNR b kq - 0 1";

	Move m2 = Move(e8, c8, OOO);
	string m2_fen = "2kr1bnr/ppp1pppp/1nbq4/8/3pP3/1NBQ2PP/PPPP1P2/2KR1BNR w - - 0 1";

	p.play<WHITE>(m1);
	CHECK_EQ(p.fen(), m1_fen);

	p.play<BLACK>(m2);
	CHECK_EQ(p.fen(), m2_fen);
	p.undo<BLACK>(m2);

	CHECK_EQ(p.fen(), m1_fen);
	p.undo<WHITE>(m1);

	CHECK_EQ(p.fen(), start_fen);
}

TEST_CASE("promotions") {
	Position p;
	SUBCASE("white promotion") {
		string start_fen = "rnbq2nr/1ppppPpp/k2b4/2p5/8/8/PPPPP1PP/RNBQKBNR w KQ - 0 1";
		p.set_fen(start_fen);

		Move m1 = Move(f7, f8, PR_QUEEN);
		string m1_fen = "rnbq1Qnr/1pppp1pp/k2b4/2p5/8/8/PPPPP1PP/RNBQKBNR b KQ - 0 1";

		p.play<WHITE>(m1);
		CHECK_EQ(p.fen(), m1_fen);
		p.undo<WHITE>(m1);

		CHECK_EQ(p.fen(), start_fen);
	}
	SUBCASE("white promotion-capture") {
		string start_fen = "rnbq2nr/1ppppPpp/k2b4/2p5/8/8/PPPPP1PP/RNBQKBNR w KQ - 0 1";
		p.set_fen(start_fen);

		Move m1 = Move(f7, g8, PR_QUEEN | CAPTURE_TYPE);
		string m1_fen = "rnbq2Qr/1pppp1pp/k2b4/2p5/8/8/PPPPP1PP/RNBQKBNR b KQ - 0 1";

		p.play<WHITE>(m1);
		CHECK_EQ(p.fen(), m1_fen);
		p.undo<WHITE>(m1);

		CHECK_EQ(p.fen(), start_fen);
	}
	SUBCASE("black promotion") {
		string start_fen = "rnbqkbnr/ppppp1pp/8/8/8/2KBP3/PPPPPpPP/RNBQ2NR b kq - 0 1";
		p.set_fen(start_fen);

		Move m1 = Move(f2, f1, PR_QUEEN);
		string m1_fen = "rnbqkbnr/ppppp1pp/8/8/8/2KBP3/PPPPP1PP/RNBQ1qNR w kq - 0 1";

		p.play<BLACK>(m1);
		CHECK_EQ(p.fen(), m1_fen);
		p.undo<BLACK>(m1);

		CHECK_EQ(p.fen(), start_fen);
	}
	SUBCASE("black promotion-capture") {
		string start_fen = "rnbqkbnr/ppppp1pp/8/8/8/2KBP3/PPPPPpPP/RNBQ2NR b kq - 0 1";
		p.set_fen(start_fen);

		Move m1 = Move(f2, g1, PR_QUEEN | CAPTURE_TYPE);
		string m1_fen = "rnbqkbnr/ppppp1pp/8/8/8/2KBP3/PPPPP1PP/RNBQ2qR w kq - 0 1";

		p.play<BLACK>(m1);
		CHECK_EQ(p.fen(), m1_fen);
		p.undo<BLACK>(m1);

		CHECK_EQ(p.fen(), start_fen);
	}
}

TEST_SUITE_END();