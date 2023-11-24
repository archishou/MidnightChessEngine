//
// Created by Archishmaan Peyyety on 8/18/23.
//
#include <atomic>
#include "datagen.h"
#include <filesystem>
#include "../engine.h"

Position datagen_random_game(std::atomic<bool>& run) {
	std::random_device dev;
	auto seed = dev();
	std::mt19937 rng(seed);
	urng heads_tails(0, 1);

	Position board{INITIAL_BOARD_FEN};
	usize num_initial_moves = heads_tails(rng) % 2 + 8;

	bool board_generated = false;
	while (!board_generated && run.load()) {
		board.set_fen(INITIAL_BOARD_FEN);
		for (usize idx = 0; idx < num_initial_moves; idx++) {
			board_generated = true;
			if (board.turn() == WHITE) {
				MoveList<WHITE, MoveGenerationType::ALL> move_list(board);

				if (move_list.size() == 0) {
					board_generated = false;
					break;
				}
				urng idxrng(0, move_list.size() - 1);
				usize ridx = idxrng(rng);
				board.play<WHITE>(move_list[ridx]);
			} else {
				MoveList<BLACK, MoveGenerationType::ALL> move_list(board);

				if (move_list.size() == 0) {
					board_generated = false;
					break;
				}
				urng idxrng(0, move_list.size() - 1);
				usize ridx = idxrng(rng);
				board.play<BLACK>(move_list[ridx]);
			}
		}
	}
	return board;
}

void single_thread_datagen(const string& output_file_path,
						   std::atomic<usize>& total_fens_collected, std::atomic<bool>& run) {
	auto tdata = std::make_unique<ThreadData>();
	SearchParameters sparams{};

	std::ofstream output_fens;
	output_fens.open(output_file_path, std::ios_base::out | std::ios_base::app);
	std::vector<DatagenGameInfo> collected_fens{};
	collected_fens.reserve(256);

	while (run.load()) {
		Position board = datagen_random_game(run);
		initialize_engine();
		init_history(*tdata);

		collected_fens.clear();
		usize legal_move_size;
		do {
			if (board.turn() == WHITE) {
				MoveList<WHITE, MoveGenerationType::ALL> move_list(board);
				legal_move_size = move_list.size();
			} else {
				MoveList<BLACK, MoveGenerationType::ALL> move_list(board);
				legal_move_size = move_list.size();
			}

			if (legal_move_size != 0) {
				SearchData sdata = {};
				sparams.soft_time_limit = sparams.hard_time_limit = 86'400'000;
				sdata.hard_node_limit = 8'000'000;
				sdata.soft_node_limit = 5'000;
				search(sdata, *tdata, board, sparams);

				if (!sdata.final_best_move.is_capture() && !board.in_check<WHITE>() && !board.in_check<BLACK>()) {
					if (board.turn() == BLACK) sdata.final_value *= -1;
					collected_fens.push_back({board.fen(), sdata.final_value});
				}

				if (board.turn() == WHITE) board.play<WHITE>(sdata.final_best_move);
				else board.play<BLACK>(sdata.final_best_move);
			}
		} while (run.load() && legal_move_size != 0 && !board.has_repetition(Position::THREE_FOLD) && board.fifty_move_rule() < 100);

		if (!run.load()) break;

		DatagenGameResults game_result;
		if (!board.has_repetition(Position::THREE_FOLD) && board.fifty_move_rule() < 100) {
			if (board.in_check<WHITE>()) game_result = DatagenGameResults::BLACK_WON;
			else if (board.in_check<BLACK>()) game_result = DatagenGameResults::WHITE_WON;
			else game_result = DatagenGameResults::DRAW;
		} else game_result = DatagenGameResults::DRAW;

		auto skip_one = false;
		for (const auto& game_info : collected_fens) {
			if (!skip_one) {
				skip_one = true;
				continue;
			}
			if (std::abs(game_info.value) >= MATE_BOUND) break;

			total_fens_collected += 1;
			if (game_result == DatagenGameResults::DRAW) {
				output_fens << game_info.fen << " | " << game_info.value << " | " << 0.5 << "\n";
				continue;
			}

			string game_score = "1.0";
			if (game_result == DatagenGameResults::BLACK_WON) game_score = "0.0";
			output_fens << game_info.fen << " | " << game_info.value << " | " << game_score << "\n";
		}
	}
	output_fens.close();
}

void datagen(const i32 thread_count, const string& output_directory) {
	std::vector<std::thread> threads{};
	std::atomic<usize> total_fens_collected{0};
	std::atomic<bool> run{true};
	for (i32 idx = 0; idx < thread_count; idx += 1) {
		std::filesystem::path root(output_directory);
		std::filesystem::path file_name("fens" + std::to_string(idx) + ".txt");
		auto output_path = root / file_name;
		std::cout << output_path << "\n";
		threads.emplace_back(
				[&output_path, &total_fens_collected, &run] {
					single_thread_datagen(output_path, total_fens_collected, run);
				});
	}

	auto check_run_state = [&run] {
		std::cout.setf(std::ios::unitbuf);
		string input_line{};
		while (std::getline(std::cin, input_line)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			if (input_line.empty()) continue;
			run.store(false);
			break;
		}
	};

	threads.emplace_back(check_run_state);

	auto dgen_start_time = std::chrono::high_resolution_clock::now();
	while (run) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		auto current_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = current_time - dgen_start_time;
		double seconds_elapsed = diff.count();
		usize fps = total_fens_collected / seconds_elapsed;
		std::cout << "Total Fens Collected: " << total_fens_collected << " FENS/S: " << fps << "\n";
	}

	for (auto& t : threads) t.join();
}
