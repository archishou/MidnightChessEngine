//
// Created by Archishmaan Peyyety on 5/16/23.
//
#include "lib/doctests.h"
#include "iostream"
#include "../src/board/position.h"
#include "../src/move_gen/move_generator.h"
#include "../src/utils/fen_constants.h"

TEST_SUITE_BEGIN("hash");

template<Color Us>
void perft_hash(Position& p, unsigned int depth) {
	MoveList<Us, ALL> list(p);
	if (depth == 1) return;
	for (Move move : list) {
		ZobristHash initial_hash = p.hash();
		Position b2(p.fen());
		CHECK_EQ(p.hash(), b2.hash());

		p.play<Us>(move);
		perft_hash<~Us>(p, depth - 1);
		p.undo<Us>(move);

		CHECK_EQ(initial_hash, p.hash());
		CHECK_EQ(initial_hash, b2.hash());
	}
}

void test_perft_hash(const std::string& fen, int depth) {
	Position p(fen);
	if (p.turn() == BLACK) perft_hash<BLACK>(p, depth);
	else perft_hash<WHITE>(p, depth);
}

TEST_CASE("side-to-move") {
	Position from_fen("rnbqkbnr/ppppp1pp/8/5p2/5P2/2N5/PPPPP1PP/R1BQKBNR b KQkq - 1 2");

	Position board_1(START_FEN);
	board_1.play<WHITE>(Move(f2, f4, DOUBLE_PUSH));
	board_1.play<BLACK>(Move(f7, f5, DOUBLE_PUSH));
	board_1.play<WHITE>(Move(b1, c3, QUIET));

	Position board_2(START_FEN);
	board_2.play<WHITE>(Move(f2, f3, QUIET));
	board_2.play<BLACK>(Move(f7, f6, QUIET));
	board_2.play<WHITE>(Move(f3, f4, QUIET));
	board_2.play<BLACK>(Move(f6, f5, QUIET));
	board_2.play<WHITE>(Move(b1, c3, QUIET));

	CHECK_EQ(from_fen.hash(), board_1.hash());
	CHECK_EQ(board_1.hash(), board_2.hash());
}

TEST_CASE("castling") {
	Position from_fen("rnbqk2r/pppppp1p/5n2/2b3p1/2B3P1/5N2/PPPPPP1P/RNBQ1RK1 b kq - 0 3");

	Position board_1("rnbqk2r/pppppppp/5n2/2b5/2B5/5N2/PPPPPPPP/RNBQK2R w KQkq - 0 1");
	Position board_2("rnbqk2r/pppppppp/5n2/2b5/2B5/5N2/PPPPPPPP/RNBQK2R w KQkq - 0 1");

	board_1.play<WHITE>(Move(g2, g4, DOUBLE_PUSH));
	board_1.play<BLACK>(Move(g7, g5, DOUBLE_PUSH));
	board_1.play<WHITE>(Move(e1, g1, OO));

	board_2.play<WHITE>(Move(e1, g1, OO));
	board_2.play<BLACK>(Move(g7, g6, QUIET));
	board_2.play<WHITE>(Move(g2, g3, QUIET));
	board_2.play<BLACK>(Move(g6, g5, QUIET));
	board_2.play<WHITE>(Move(g3, g4, QUIET));
	CHECK_EQ(from_fen.hash(), board_1.hash());
	CHECK_EQ(board_1.hash(), board_2.hash());
}

TEST_CASE("en-passant") {
	Position from_fen("rnbqkbnr/pppp1pp1/8/3Pp2p/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 4");

	Position board_1(START_FEN);
	Position board_2(START_FEN);

	board_1.play<WHITE>(Move(d2, d3, QUIET));
	board_1.play<BLACK>(Move(h7, h6, QUIET));
	board_1.play<WHITE>(Move(d3, d4, QUIET));
	board_1.play<BLACK>(Move(h6, h5, QUIET));
	board_1.play<WHITE>(Move(d4, d5, QUIET));
	board_1.play<BLACK>(Move(e7, e5, DOUBLE_PUSH));

	board_2.play<WHITE>(Move(d2, d4, DOUBLE_PUSH));
	board_2.play<BLACK>(Move(h7, h5, DOUBLE_PUSH));
	board_2.play<WHITE>(Move(d4, d5, QUIET));
	board_2.play<BLACK>(Move(e7, e5, DOUBLE_PUSH));

	CHECK_EQ(from_fen.hash(), board_1.hash());
	CHECK_EQ(board_1.hash(), board_2.hash());
}

TEST_CASE("TestConsitentHashInitialFen") {
	test_perft_hash(START_FEN, 6);
}
TEST_CASE("TestConsitentHashKiwiPete") {
	test_perft_hash(KIWIPETE_FEN, 5);
}
TEST_CASE("TestConsitentHashTalkchess") {
	test_perft_hash(TALKCHESS_FEN, 5);
}

TEST_CASE("TestTraiangulation") {
	Position p;
	p.set_fen("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1");
	ZobristHash initial_hash = p.hash();

	Move m1 = Move(a1, a2, QUIET);
	Move m2 = Move(a7, b7, QUIET);
	Move m3 = Move(a2, b2, QUIET);
	Move m4 = Move(b7, a7, QUIET);
	Move m5 = Move(b2, a1, QUIET);

	p.play<WHITE>(m1);
	p.play<BLACK>(m2);
	p.play<WHITE>(m3);
	p.play<BLACK>(m4);
	p.play<WHITE>(m5);
	CHECK_NE(p.hash(), initial_hash);

	p.undo<WHITE>(m5);
	p.undo<BLACK>(m4);
	p.undo<WHITE>(m3);
	p.undo<BLACK>(m2);
	p.undo<WHITE>(m1);
	CHECK_EQ(p.hash(), initial_hash);
}

TEST_CASE("TestCastling") {
	Position p(INITIAL_BOARD_FEN);
	ZobristHash initial_hash = p.hash();

	Move m1 = Move(e2, e4, DOUBLE_PUSH);
	Move m2 = Move(e7, e5, DOUBLE_PUSH);
	Move m3 = Move(g1, f3);
	Move m4 = Move(g8, f6);
	Move m5 = Move(f1, d3);
	Move m6 = Move(f8, d6);
	Move m7 = Move(e1, g1, OO);

	p.play<WHITE>(m1);
	p.play<BLACK>(m2);
	p.play<WHITE>(m3);
	p.play<BLACK>(m4);
	p.play<WHITE>(m5);
	p.play<BLACK>(m6);
	p.play<WHITE>(m7);

	Position p2(p.fen());

	CHECK_EQ(p.hash(), p2.hash());

	p.undo<WHITE>(m7);
	p.undo<BLACK>(m6);
	p.undo<WHITE>(m5);
	p.undo<BLACK>(m4);
	p.undo<WHITE>(m3);
	p.undo<BLACK>(m2);
	p.undo<WHITE>(m1);

	Position p3(p.fen());
	CHECK_EQ(p.hash(), initial_hash);
	CHECK_EQ(p3.hash(), initial_hash);
}

TEST_CASE("TestEPFile") {
	Position p;

	p.set_fen(INITIAL_BOARD_FEN);
	ZobristHash initial_hash = p.hash();
	Move m1 = Move(e2, e4, DOUBLE_PUSH);
	Move m2 = Move(d7, d5, DOUBLE_PUSH);
	Move m3 = Move(e4, e5);
	Move m4 = Move(f7, f5, DOUBLE_PUSH);
	p.play<WHITE>(m1);
	p.play<BLACK>(m2);
	p.play<WHITE>(m3);
	p.play<BLACK>(m4);
	Position p2(p.fen());

	CHECK_EQ(p2.hash(), p.hash());

	p.undo<BLACK>(m4);
	p.undo<WHITE>(m3);
	p.undo<BLACK>(m2);
	p.undo<WHITE>(m1);
	CHECK_EQ(p.hash(), initial_hash);
}

TEST_CASE("capture-hash") {
	Position p(START_FEN);
	p.play<WHITE>(Move(b1, a3, QUIET));
	p.play<BLACK>(Move(b8, a6, QUIET));
	p.play<WHITE>(Move(b2, b4, DOUBLE_PUSH));
	p.play<BLACK>(Move(a6, b4, CAPTURE_TYPE));

	Position p2("r1bqkbnr/pppppppp/8/8/1n6/N7/P1PPPPPP/R1BQKBNR w KQkq - 0 3");

	std::cout << p2.fen() << std::endl;
	std::cout << p.fen() << std::endl;

	std::cout << p2.hash() << std::endl;
	std::cout << p.hash() << std::endl;
}

TEST_CASE("promo-hash") {
	Position final("rnbq1bQr/ppppp1pp/2k4p/8/8/8/PPPPP1PP/RNBQKBNR b KQ - 0 1");

	Position step_before("rnbq1bnr/pppppPpp/2k4p/8/8/8/PPPPP1PP/RNBQKBNR w KQ - 0 1");
	step_before.play<WHITE>(Move(f7, g8, PR_QUEEN | CAPTURE_TYPE));

	std::cout << final.fen() << std::endl;
	std::cout << step_before.fen() << std::endl;

	std::cout << final.hash() << std::endl;
	std::cout << step_before.hash() << std::endl;
}

TEST_CASE("kiwipete-fen") {
	Position from_fen(KIWIPETE_FEN);
	from_fen.play<WHITE>(Move(e1, d1, QUIET));
	from_fen.play<BLACK>(Move(c7, c5, DOUBLE_PUSH));
	from_fen.play<WHITE>(Move(d5, c6, ENPASSANT));

	Position as_fen("r3k2r/p2pqpb1/bnP1pnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R2K3R b kq - 0 2");

	CHECK_EQ(from_fen.hash(), as_fen.hash());
}

TEST_SUITE_END();