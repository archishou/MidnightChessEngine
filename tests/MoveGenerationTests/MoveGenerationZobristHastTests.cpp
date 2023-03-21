//
// Created by Archishmaan Peyyety on 1/22/23.
//

#include "../lib/doctests.h"
#include "utils/fen_constants.h"
#include "move_generation/position.h"
#include "engine.h"

template<Color Us>
void perft_hash(Position& p, unsigned int depth) {
	MoveList<Us> list(p);
	if (depth == 1) return;
	for (Move move : list) {
		zobrist_hash initial_hash = p.get_hash();
		Position b2;
		Position::set(p.fen(), b2);
		CHECK_EQ(p.get_hash(), b2.get_hash());
		if (initial_hash != b2.get_hash()) {
			std::cout << "Failure to set position! " << std::endl;
		}
		p.play_null<~Us>();
		p.play_null<Us>();
		perft_hash<~Us>(p, depth - 1);
		p.undo_null<Us>();
		p.undo_null<~Us>();
		CHECK_EQ(initial_hash, p.get_hash());
		CHECK_EQ(initial_hash, b2.get_hash());
	}
}

void test_perft_hash(const std::string& fen, int depth) {
	Position p;
	Position::set(fen, p);
	if (p.turn() == BLACK) perft_hash<BLACK>(p, depth);
	else perft_hash<WHITE>(p, depth);
}

TEST_SUITE_BEGIN("Zobrist Hash Test Suite");

TEST_CASE("TestCastlingState") {
	Position board;

	for (int i = 15; i >= 0; i--) {
		std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w ";
		std::string castling_rights = "";
		std::string move_counter = " 0 1";
		if ((i >> 3) & 0b1)	castling_rights += "K";
		if ((i >> 2) & 0b1)	castling_rights += "Q";
		if ((i >> 1) & 0b1)	castling_rights += "k";
		if (i & 0b1) castling_rights += "q";
		if (i == 0) castling_rights = "-";
		std::string test_fen = fen + castling_rights + move_counter;
		Position::set(test_fen, board);
		CHECK_EQ(board.castling_state(), i);
	}
}
TEST_CASE("TestConsitentHashInitialFen") {
	initialize_engine();
	test_perft_hash(INITIAL_BOARD_FEN, 6);
}
TEST_CASE("TestConsitentHashKiwiPete") {
	initialize_engine();
	test_perft_hash(KIWIPETE_FEN, 5);
}
TEST_CASE("TestConsitentHashTalkchess") {
	initialize_engine();
	test_perft_hash(TALKCHESS_FEN, 5);
}

TEST_CASE("TestTraiangulation") {
	initialize_engine();
	Position p;
	Position::set("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1", p);
	zobrist_hash initial_hash = p.get_hash();

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
	CHECK_NE(p.get_hash(), initial_hash);

	p.undo<WHITE>(m5);
	p.undo<BLACK>(m4);
	p.undo<WHITE>(m3);
	p.undo<BLACK>(m2);
	p.undo<WHITE>(m1);
	CHECK_EQ(p.get_hash(), initial_hash);
}

TEST_CASE("TestCastling") {
	initialize_engine();
	Position p;
	Position::set(INITIAL_BOARD_FEN, p);
	zobrist_hash initial_hash = p.get_hash();

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

	Position p2;
	Position::set(p.fen(), p2);

	CHECK_EQ(p.get_hash(), p2.get_hash());

	p.undo<WHITE>(m7);
	p.undo<BLACK>(m6);
	p.undo<WHITE>(m5);
	p.undo<BLACK>(m4);
	p.undo<WHITE>(m3);
	p.undo<BLACK>(m2);
	p.undo<WHITE>(m1);

	Position p3;
	Position::set(p.fen(), p3);
	CHECK_EQ(p.get_hash(), initial_hash);
	CHECK_EQ(p3.get_hash(), initial_hash);
}

TEST_CASE("TestEPFile") {
	initialize_engine();
	Position p;
	Position::set("rnbqkbnr/1ppppppp/8/pP6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6 0 1", p);
	CHECK_EQ(p.ep_file(), 0);
	Position::set("rnbqkbnr/p1pppppp/8/1pP5/8/8/PP1PPPPP/RNBQKBNR w KQkq b6 0 1", p);
	CHECK_EQ(p.ep_file(), 1);
	Position::set("rnbqkbnr/pp1ppppp/8/2pP4/8/8/PPP1PPPP/RNBQKBNR w KQkq c6 0 1", p);
	CHECK_EQ(p.ep_file(), 2);
	Position::set("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", p);
	CHECK_EQ(p.ep_file(), 3);
	Position::set("rnbqkbnr/pppp1ppp/8/4pP2/8/8/PPPPP1PP/RNBQKBNR w KQkq e6 0 1", p);
	CHECK_EQ(p.ep_file(), 4);
	Position::set("rnbqkbnr/ppppp1pp/8/5pP1/8/8/PPPPPP1P/RNBQKBNR w KQkq f6 0 1", p);
	CHECK_EQ(p.ep_file(), 5);
	Position::set("rnbqkbnr/pppppp1p/8/6pP/8/8/PPPPPPP1/RNBQKBNR w KQkq g6 0 1", p);
	CHECK_EQ(p.ep_file(), 6);
	Position::set("rnbqkbnr/ppppppp1/8/6Pp/8/8/PPPPPP1P/RNBQKBNR w KQkq h6 0 1", p);
	CHECK_EQ(p.ep_file(), 7);
	Position::set(INITIAL_BOARD_FEN, p);
	CHECK_EQ(p.ep_file(), 8);


	Position::set(INITIAL_BOARD_FEN, p);
	zobrist_hash initial_hash = p.get_hash();
	Move m1 = Move(e2, e4, DOUBLE_PUSH);
	Move m2 = Move(d7, d5, DOUBLE_PUSH);
	Move m3 = Move(e4, e5);
	Move m4 = Move(f7, f5, DOUBLE_PUSH);
	p.play<WHITE>(m1);
	p.play<BLACK>(m2);
	p.play<WHITE>(m3);
	p.play<BLACK>(m4);
	Position p2;
	Position::set(p.fen(), p2);
	CHECK_EQ(p2.get_hash(), p.get_hash());

	p.undo<BLACK>(m4);
	p.undo<WHITE>(m3);
	p.undo<BLACK>(m2);
	p.undo<WHITE>(m1);
	CHECK_EQ(p.get_hash(), initial_hash);
}

TEST_SUITE_END();