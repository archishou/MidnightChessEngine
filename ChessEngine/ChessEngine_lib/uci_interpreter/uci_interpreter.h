//
// Created by Archishmaan Peyyety on 1/9/23.
//
#include <algorithm>
#include <iostream>
#include "move_generation/tables.h"
#include "move_generation/position.h"
#include "move_generation/types.h"
#include "move_search/search.h"
#include "helpers.h"
#include "constants.h"

using namespace std;

void initialize_uci(Position& p) {
	initialise_all_databases();
	zobrist::initialise_zobrist_keys();
	Position::set(INITIAL_BOARD_FEN, p);
}

char promotion_character(std::string uci_move) {
	char promotion_piece = uci_move.at(4);
	char lower_case = std::tolower(promotion_piece, std::locale());
	return lower_case;
}

Move uci_to_move(const std::string& moveStr, Position& position) {
	Move move = Move(moveStr.substr(0, 4));
	// Pawn Promotion
	if (moveStr.size() == 5) {
		// Quiet Promotion
		char p_char = promotion_character(moveStr);
		if (position.at(move.to()) == NO_PIECE) {
			if (p_char == 'q') return Move(move.from(), move.to(), PR_QUEEN);
			if (p_char == 'b') return Move(move.from(), move.to(), PR_BISHOP);
			if (p_char == 'n') return Move(move.from(), move.to(), PR_KNIGHT);
			if (p_char == 'r') return Move(move.from(), move.to(), PR_ROOK);
		}
		if (p_char == 'q') return Move(move.from(), move.to(), PC_QUEEN);
		if (p_char == 'b') return Move(move.from(), move.to(), PC_BISHOP);
		if (p_char == 'n') return Move(move.from(), move.to(), PC_KNIGHT);
		if (p_char == 'r') return Move(move.from(), move.to(), PC_ROOK);
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

void uci_update_position_from_moves(Position& board, const string& uci_move_string) {
	vector<string> uci_moves = split(uci_move_string, " ");
	for (const std::string& uci_move : uci_moves) {
		if (uci_move.empty()) return;
		Move nextMove = uci_to_move(uci_move, board);
		if (board.turn() == BLACK) board.play<BLACK>(nextMove);
		else board.play<WHITE>(nextMove);
	}
}

void uci_position(Position& board, const string& input_line) {
	if (input_line.substr(0, 17) == "position startpos") {
		std::string uci_moves;
		if (input_line.size() > 17) uci_moves = input_line.substr(24, input_line.size() - 24);
		Position::set(INITIAL_BOARD_FEN, board);
		uci_update_position_from_moves(board,  uci_moves);
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

		Position::set(fen, board);
		uci_update_position_from_moves(board, moves);
	}
}

void uci_go_diagnostics_output(Position& board, BestMoveSearchResults& results, ofstream& diagnostics_file)  {
	diagnostics_file << "bestmove " << results.best_move << endl;

	/*
	//diagnostics_file << "Position FEN : " << board.fen() << std::endl;
	//diagnostics_file << "Predicted Best Move: " << results.best_move << std::endl;
	//diagnostics_file << "Principal Variation " << results.pv << ":" << std::endl;
	//diagnostics_file << "Depth Searched: " << results.depth_searched << std::endl;
	//diagnostics_file << "Time Searched: " << results.time_searched << std::endl;
	//diagnostics_file << "Nodes Searched: " << results.nodes_searched << std::endl;
	//diagnostics_file << "NPS: " << results.nodes_per_second << std::endl;
	 */
	////diagnostics_file << "Value: " << results.value << std::endl;
}

void uci_go(Position& board, ofstream& diagnostics_file, const string& input_line) {

	auto t_start = std::chrono::high_resolution_clock::now();

	BestMoveSearchResults results;
	string move_time_s = split(input_line," ")[2];
	int move_time = stoi(move_time_s);
	const BestMoveSearchParameters params = BestMoveSearchParameters {
		.depth = MAX_DEPTH,
		.time_limit = move_time,
	};
	results = best_move(board, params);
	uci_go_diagnostics_output(board, results, diagnostics_file);

	auto t_end = std::chrono::high_resolution_clock::now();
	auto millis = std::chrono::duration<double, std::milli>(t_end-t_start).count();

	std::cout << "OVERALL TIME: " << millis << std::endl;
	cout << "info score cp " << results.value << endl;
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
			//diagnostics_file << "id name MidnightChessEngine" << std::endl;
			//diagnostics_file << "id author Archishmaan Peyyety" << std::endl;
			//diagnostics_file << "uciok" << std::endl;
			cout << "id name MidnightChessEngine" << endl;
			cout << "id author Archishmaan Peyyety" << endl;
			cout << "uciok" << endl;
		} else if (input_line == "quit") {
			//diagnostics_file << "Bye Bye" << std::endl;
			cout << "Bye Bye" << endl;
			break;
		} else if (input_line == "isready") {
			//diagnostics_file << "readyok" << std::endl;
			cout << "readyok" << endl;
		} else if (input_line == "ucinewgame") {}
		if (input_line.substr(0, 8) == "position") {
			uci_position(board, input_line);
		} else if (input_line == "stop") {
		} else if (input_line.substr(0, 2 ) == "go") {
			uci_go(board, diagnostics_file, input_line);
		}
	}
	diagnostics_file.close();
}

void read_uci_from_file(const string& input_file_path, const string& output_file_path) {
	Position board;
	initialize_uci(board);

	std::ifstream input_file(input_file_path);
	std::string input_line;

	ofstream output_file;
	output_file.open(output_file_path);

	while (std::getline(input_file, input_line)) {
		output_file << input_line << std::endl;
		if (input_line == "uci") {
			output_file << "id name MidnightChessEngine" << endl;
			output_file << "id author Archishmaan Peyyety" << endl;
			output_file << "uciok" << endl;
		} else if (input_line == "quit") {
			output_file << "Bye Bye" << endl;
			break;
		} else if (input_line == "isready") {
			output_file << "readyok" << endl;
		} else if (input_line == "ucinewgame") {}
		if (input_line.substr(0, 8) == "position") {
			uci_position(board, input_line);
		} else if (input_line == "stop") {
		} else if (input_line.substr(0, 2 ) == "go") {
			uci_go(board, output_file, input_line);
		}
	}
	input_file.close();
}