//
// Created by Archishmaan Peyyety on 1/9/23.
//
#include "move_search/search.h"
#include "parse_uci_move.h"
#include "helpers.h"
#include "constants.h"
#include "sstream"

using namespace std;

struct ReadUCIParameters {
	std::string diagnostic_file_path;
	std::string uci_file_input_path;
	std::string uci_file_output_path;
	bool read_from_file = false;
	bool output_diagnostic_file = false;
};

struct UCIStreams {
	std::ifstream input_file;
	ofstream diagnostics_file;
	ofstream output_file;
};

enum OutputLocations {
	UCIOutputFile,
	DiagnosticFile,
	StdOut,
};

bool using_diagnostic_file(ReadUCIParameters& parameters) {
	return !parameters.diagnostic_file_path.empty() && parameters.output_diagnostic_file;
}

bool using_file_io(ReadUCIParameters& parameters) {
	return (!parameters.uci_file_input_path.empty() && !parameters.uci_file_output_path.empty()
		&& parameters.read_from_file);
}

void initialize_uci_streams(UCIStreams& streams, ReadUCIParameters& parameters) {
	if (using_diagnostic_file(parameters)) {
		streams.diagnostics_file.open(parameters.diagnostic_file_path);
	}
	if (using_file_io(parameters)) {
		streams.input_file = std::ifstream(parameters.uci_file_input_path);
		streams.output_file.open(parameters.uci_file_output_path);
	}
}

void close_streams(UCIStreams& streams, ReadUCIParameters& parameters) {
	if (using_diagnostic_file(parameters)) {
		streams.diagnostics_file.close();
		streams.diagnostics_file.open(parameters.diagnostic_file_path);
	}
	if (using_file_io(parameters)) {
		streams.input_file.close();
		streams.output_file.close();
	}
}

bool get_input(std::string &input_line, UCIStreams &streams, ReadUCIParameters &parameters) {
	if (using_file_io(parameters)) {
		if (std::getline(streams.input_file, input_line)) return true;
	} else {
		if (std::getline(cin, input_line)) return true;
	}
	return false;
}

void send_output(const std::string& message, UCIStreams& streams, ReadUCIParameters& parameters, bool send_to_std) {
	if (send_to_std) std::cout << message << std::endl;
	if (using_diagnostic_file(parameters)) {
		streams.diagnostics_file << message << std::endl;
	}
	if (using_file_io(parameters)) {
		streams.output_file << message << std::endl;
	}
}


void send_output(const std::string& message, UCIStreams& streams, ReadUCIParameters& parameters) {
	send_output(message, streams, parameters, true);
}

void initialize_uci(Position& p) {
	initialise_all_databases();
	zobrist::initialise_zobrist_keys();
	Position::set(INITIAL_BOARD_FEN, p);
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

void uci_go(Position& board, const string& input_line, UCIStreams& streams, ReadUCIParameters& parameters) {
	auto t_start = std::chrono::high_resolution_clock::now();

	BestMoveSearchResults results;
	string move_time_s = split(input_line," ")[2];
	int move_time = stoi(move_time_s);
	const BestMoveSearchParameters params = BestMoveSearchParameters {
		.depth = MAX_DEPTH,
		.time_limit = move_time,
		.debug_info = parameters.output_diagnostic_file
	};
	results = best_move(board, params);
	std::cout << "bestmove " << results.best_move << std::endl;
}

void read_uci(ReadUCIParameters& parameters) {
	Position board;
	UCIStreams streams;

	initialize_uci_streams(streams, parameters);
	initialize_uci(board);

	string input_line; //to read the command given by the GUI

	cout.setf(ios::unitbuf);// Make sure that the outputs are sent straight away to the GUI

	while (get_input(input_line, streams, parameters)) {
		send_output(input_line, streams, parameters, false);
		if (input_line == "uci") {
			send_output("id name Midnight", streams, parameters);
			send_output("id author Archishmaan Peyyety", streams, parameters);
			send_output("uciok", streams, parameters);
		} else if (input_line == "quit") {
			send_output("Bye Bye", streams, parameters);
			break;
		} else if (input_line == "isready") {
			send_output("readyok", streams, parameters);
		} else if (input_line == "ucinewgame") {}
		if (input_line.substr(0, 8) == "position") {
			uci_position(board, input_line);
		} else if (input_line == "stop") {
		} else if (input_line.substr(0, 2 ) == "go") {
			uci_go(board, input_line, streams, parameters);
		}
	}
	close_streams(streams, parameters);
}
