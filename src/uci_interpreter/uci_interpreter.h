//
// Created by Archishmaan Peyyety on 1/9/23.
//
#pragma once
#include "engine.h"
#include "parse_uci_move.h"
#include "time_manager.h"
#include "sstream"
#include "bench_fens.h"
#include "../utils/fen_constants.h"
#include "../utils/helpers.h"
#include "../move_search/search.h"

struct ReadUCIParameters {
	bool debug_info = true;
};

inline void initialize_uci(Position& p) {
	initialize_engine();
	p.set_fen(INITIAL_BOARD_FEN);
}

inline void uci_position(Position& board, const std::string& input_line) {
	if (input_line.substr(0, 17) == "position startpos") {
		std::string uci_moves;
		if (input_line.size() > 17) uci_moves = input_line.substr(24, input_line.size() - 24);
		board.set_fen(INITIAL_BOARD_FEN);
		uci_update_position_from_moves(board,  uci_moves);
	} else {
		int fen_start = static_cast<int>(input_line.find("position fen ")) + 13;
		int fen_end = static_cast<int>(input_line.find(" moves"));
		int moves_start = fen_end + 6;
		int fen_size = fen_end - fen_start;
		const std::string& fen = input_line.substr(fen_start, fen_size);
		std::string moves;
		if (input_line.find(" moves") != std::string::npos) {
			moves = input_line.substr(moves_start + 1, input_line.size() - moves_start);
		}

		board.set_fen(fen);
		uci_update_position_from_moves(board, moves);
	}
}

inline void parse_move_time(const Color side_to_play, const std::string& move_time_s, BestMoveSearchParameters& params) {
	std::vector<std::string> tokens = split(move_time_s, " ");
	// Possible inputs to parse
	// input --> go movetime xxx
	// input --> go xtime ### xinc ### ytime ### yinc ###
	// input --> go xtime ### ytime ###
	if (tokens[1] == "movetime") {
		params.hard_time_limit = stoi(tokens[2]) * 0.95;
		params.soft_time_limit = stoi(tokens[2]) * 0.95;
		return;
	}
	int wtime = 0, winc = 0, btime = 0, binc = 0, moves_to_go = -1;
	for (int i = 1; i < static_cast<int>(tokens.size()); i += 2) {
		std::string token = tokens[i];
		int value = 0;
		if (static_cast<int>(tokens.size()) > i + 1) value = stoi(tokens[i + 1]);

		if (token == "wtime") wtime = value;
		else if (token == "winc") winc = value;
		else if (token == "btime") btime = value;
		else if (token == "binc") binc = value;
		else if (token == "movestogo") moves_to_go = value;
	}
	if (side_to_play == WHITE) {
		params.soft_time_limit = time_search(wtime, winc, moves_to_go);
		params.hard_time_limit = time_iterative_deepening(wtime, winc, moves_to_go);
		return;
	}
	params.soft_time_limit = time_search(btime, binc, moves_to_go);
	params.hard_time_limit = time_iterative_deepening(btime, binc, moves_to_go);
}

inline void uci_go(Position& board, const std::string& input_line, ReadUCIParameters& uci_parameters) {
	BestMoveSearchResults results;
	BestMoveSearchParameters params = BestMoveSearchParameters {
		.depth = MAX_DEPTH,
		.debug_info = uci_parameters.debug_info
	};
	parse_move_time(board.turn(), input_line, params);
	results = best_move(board, params);
	std::cout << "bestmove " << results.best_move << std::endl;
}

inline void bench() {
	uint64_t total_nodes = 0;
	auto start = std::chrono::steady_clock::now();
	for (int idx = 0; idx < BENCH_SIZE; idx++) {
		Position p;
		initialize_uci(p);
		p.set_fen(BENCH_FENS[idx]);

		std::cout << "\nPosition: " << idx + 1 << " " << BENCH_FENS[idx] << std::endl;

		BestMoveSearchParameters parameters = {
				.depth = 14,
				.hard_time_limit = 86'400'000, // 1 Day
				.soft_time_limit = 86'400'000,
				.debug_info = true,
		};
		BestMoveSearchResults results = best_move(p, parameters);
		total_nodes += results.nodes_searched;
	}
	auto end = std::chrono::steady_clock::now();
	auto total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	double total_time = static_cast<double>(total_time_ms) / 1000.0;
	std::cout << "Total Time: " << total_time << std::endl;
	std::cout << "\n";
	std::cout << total_nodes << " nodes " << signed(total_nodes / (total_time + 1)) << " nps" << std::endl;
}

template<Color Us, bool bulk>
unsigned long long perft(Position& p, unsigned int depth) {
	unsigned long long nodes = 0;

	MoveList<Us, ALL> list(p);

	if (bulk && depth == 1) return static_cast<unsigned long long>(list.size());
	if (!bulk && depth == 0) return 1;

	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft<~Us, bulk>(p, depth - 1);
		p.undo<Us>(move);
	}

	return nodes;
}

template<Color Us, bool bulk>
unsigned long long split_perft(Position& p, unsigned int depth) {
	unsigned long long nodes = 0, pf;

	MoveList<Us, ALL> list(p);

	for (Move move : list) {
		std::cout << move;
		p.play<Us>(move);
		pf = perft<~Us, bulk>(p, depth - 1);
		std::cout << ": " << pf << " moves\n";
		nodes += pf;
		p.undo<Us>(move);
	}

	return nodes;
}

template<bool bulk>
void test_perft(Position& p, int depth) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	unsigned long long n = 0;
	if (p.turn() == BLACK) n = split_perft<BLACK, bulk>(p, depth);
	else n = split_perft<WHITE, bulk>(p, depth);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto diff = end - begin;

	std::cout << "Nodes: " << n << "\n";
	std::cout << "NPS: "
			  << static_cast<int>(n * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(diff).count())
			  << "\n";
	std::cout << "Total Time = "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " millseconds\n";
}

inline void read_uci() {
	Position board;
	ReadUCIParameters parameters = {};
	initialize_uci(board);

	std::string input_line;

	std::cout.setf(std::ios::unitbuf);

	while (std::getline(std::cin, input_line)) {
		if (input_line == "uci") {
			std::cout << "id name Midnight 6.0" << std::endl;
			std::cout << "id author Archishmaan Peyyety" << std::endl;
			std::cout << "option name Hash type spin default 64 min 1 max 1024" << std::endl;
			std::cout << "option name Threads type spin default 1 min 1 max 1" << std::endl;
			std::cout << "uciok" << std::endl;
		} else if (input_line == "quit") {
			std::cout << "Bye Bye" << std::endl;
			break;
		} else if (input_line == "isready") {
			std::cout << "readyok" << std::endl;
		} else if (input_line == "ucinewgame") {
			initialize_uci(board);
		}
		if (input_line.substr(0, 8) == "position") {
			uci_position(board, input_line);
		} else if (input_line == "stop") {
		} else if (input_line == "debug on") {
			parameters.debug_info = true;
		} else if (input_line == "debug off") {
			parameters.debug_info = false;
		} else if (input_line.substr(0, 2 ) == "go") {
			uci_go(board, input_line, parameters);
		} else if (input_line == "bench") {
			bench();
		} else if (input_line.substr(0, 10) == "splitperft") {
			int depth = std::stoi(split(input_line, " ")[1]);
			test_perft<false>(board, depth);
		} else if (input_line.substr(0, 15) == "bulk splitperft") {
			int depth = std::stoi(split(input_line, " ")[2]);
			test_perft<true>(board, depth);
		} else if (input_line.substr(0, 14) == "setoption name") {
			std::vector<std::string> parsed_options = split(input_line, " ");
			std::transform(parsed_options[2].begin(), parsed_options[2].end(), parsed_options[2].begin(),
											   [](unsigned char c){ return std::tolower(c); });
			if (parsed_options[2] == "hash") {
				int mb = std::stoi(parsed_options[4]);
				t_table.resize(mb);
			}
		} else if (input_line == "hash size") {
			std::cout << t_table.entry_count() << " entries" << std::endl;
		}
	}
}
