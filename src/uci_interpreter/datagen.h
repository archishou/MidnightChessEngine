#pragma once
#include <thread>
#include <ranges>
#include <random>
#include <fstream>
#include "../move_search/pvs.h"
#include "../types.h"
#include "../utils/fen_constants.h"

using urng = std::uniform_int_distribution<std::mt19937::result_type>;

struct DatagenGameInfo {
	string fen;
	i32 value;
};

enum class DatagenGameResults {
	WHITE_WON,
	BLACK_WON,
	DRAW
};

Position datagen_random_game(std::atomic<bool>& run);
void single_thread_datagen(const string& output_file_path,
						   std::atomic<usize>& total_fens_collected, std::atomic<bool>& run);
void datagen(const i32 thread_count, const string& output_directory);
