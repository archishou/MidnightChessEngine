//
// Created by Archishmaan Peyyety on 1/9/23.
//
#include <algorithm>
#include <iostream>
#include "move_generation/tables.h"
#include "move_generation/position.h"
#include "move_generation/types.h"
#include "move_search/search.h"

using namespace std;

const std::string& initial_board_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void initialize_uci(Position& p) {
	initialise_all_databases();
	zobrist::initialise_zobrist_keys();
	Position::set(initial_board_fen, p);
}

Move uci_to_move(const std::string& moveStr, Position& position) {
	Move move = Move(moveStr.substr(0, 4));
	// Pawn Promotion
	if (moveStr.size() == 5) {
		// Quiet Promotion
		if (position.at(move.to()) == NO_PIECE) {
			if (moveStr.at(4) == 'q') return Move(move.from(), move.to(), PR_QUEEN);
			if (moveStr.at(4) == 'b') return Move(move.from(), move.to(), PR_BISHOP);
			if (moveStr.at(4) == 'n') return Move(move.from(), move.to(), PR_KNIGHT);
			if (moveStr.at(4) == 'r') return Move(move.from(), move.to(), PR_ROOK);
		}
		if (moveStr.at(4) == 'q') return Move(move.from(), move.to(), PC_QUEEN);
		if (moveStr.at(4) == 'b') return Move(move.from(), move.to(), PC_BISHOP);
		if (moveStr.at(4) == 'n') return Move(move.from(), move.to(), PC_KNIGHT);
		if (moveStr.at(4) == 'r') return Move(move.from(), move.to(), PC_ROOK);
	}

	// En Passant
	if (position.at(move.to()) == NO_PIECE && type_of(position.at(move.from())) == PAWN &&
		file_of(move.to()) != file_of(move.from())) {
		return Move(move.from(), move.to(), EN_PASSANT);
	}

	if (type_of(position.at(move.from())) == PAWN && rank_of(move.to()) - rank_of(move.from()) == 2) {
		return Move(move.from(), move.to(), DOUBLE_PUSH);
	}

	// Castle
	if (type_of(position.at(move.from())) == KING) {
		if (moveStr == "e1g1" || moveStr == "e8g8") return Move(move.from(), move.to(), OO);
		if (moveStr == "e1c1" || moveStr == "e8c8") return Move(move.from(), move.to(), OOO);
	}

	// Capture
	if (position.at(move.to()) != NO_PIECE) {
		return Move(move.from(), move.to(), CAPTURE);
	}

	return {move.from(), move.to(), QUIET};
}

vector<string> split(const string& s, const string& delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
}


void uci_create_position_from_moves(Position& board, const string& board_fen, const string& uci_move_string) {
	Position::set(board_fen, board);
	vector<string> uci_moves = split(uci_move_string, " ");
	for (const std::string& uci_move : uci_moves) {
		if (uci_move.empty()) return;
		std::cout << uci_move << std::endl;
		Move nextMove = uci_to_move(uci_move, board);
		if (board.turn() == BLACK) board.play<BLACK>(nextMove);
		else board.play<WHITE>(nextMove);
	}
}

void uci_position(Position& board, const string& input_line) {
	if (input_line.substr(0, 23) == "position startpos moves") {
		const string& uci_moves = input_line.substr(24, input_line.size() - 24);
		uci_create_position_from_moves(board, initial_board_fen, uci_moves);
	} else {
		int fen_start = input_line.find("position fen ") + 13;
		int fen_end = input_line.find(" moves");
		int moves_start = fen_end + 6;
		int fen_size = fen_end - fen_start;
		const string& fen = input_line.substr(fen_start, fen_size);
		std::string moves;
		if (fen_end != std::string::npos) {
			moves = input_line.substr(moves_start + 1, input_line.size() - moves_start);
		}
		std::cout << "ParsedFEN:" << fen << ":" << std::endl;
		std::cout << "ParsedMoves:" << moves << ":" << std::endl;
		uci_create_position_from_moves(board, fen, moves);
		std::cout << "FINAL PARSED " << std::endl;
		std::cout << board << std::endl;
	}
	std::cout << "EXPECTED " << std::endl;
	Position p;
	Position::set("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", p);
	std::cout << p << std::endl;
}

void uci_go_diagnostics_output(Position& board, BestMoveSearchResults& results, ofstream& diagnostics_file)  {
	diagnostics_file << "Position FEN : " << board.fen() << std::endl;
	diagnostics_file << "Predicted Best Move: " << results.best_move << std::endl;
	diagnostics_file << "Principal Variation " << results.pv << ":" << std::endl;
	diagnostics_file << "Depth Searched: " << results.depth_searched << std::endl;
	diagnostics_file << "Time Searched: " << results.time_searched << std::endl;
	diagnostics_file << "Nodes Searched: " << results.nodes_searched << std::endl;
	diagnostics_file << "NPS: " << results.nodes_per_second << std::endl;
	diagnostics_file << "Value: " << results.value << std::endl;
}

void uci_go(Position& board, ofstream& diagnostics_file) {
	BestMoveSearchResults results;
	if (board.turn() == BLACK) results = best_move<BLACK>(board);
	else results = best_move<WHITE>(board);
	uci_go_diagnostics_output(board, results, diagnostics_file);
	cout << "bestmove " << results.best_move << endl;
}

void read_uci(const string& diagnostics_file_path) {
	Position board;
	initialize_uci(board);

	string input_line; //to read the command given by the GUI

	cout.setf(ios::unitbuf);// Make sure that the outputs are sent straight away to the GUI
	ofstream diagnostics_file;
	diagnostics_file.open(diagnostics_file_path);

	while (getline(cin, input_line)) {
		diagnostics_file << input_line << std::endl;
		if (input_line == "uci") {
			cout << "id name MidnightChessEngine" << endl;
			cout << "id author Archishmaan Peyyety" << endl;
			cout << "uciok" << endl;
		} else if (input_line == "quit") {
			cout << "Bye Bye" << endl;
			break;
		} else if (input_line == "isready") {
			cout << "readyok" << endl;
		} else if (input_line == "ucinewgame") {}
		if (input_line.substr(0, 8) == "position") {
			uci_position(board, input_line);
		} else if (input_line == "stop") {
		} else if (input_line.substr(0, 2 ) == "go") {
			uci_go(board, diagnostics_file);
		}
	}
	diagnostics_file.close();
}