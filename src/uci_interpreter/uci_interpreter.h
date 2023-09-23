//
// Created by Archishmaan Peyyety on 1/9/23.
//
#pragma once
#include <tuple>
#include <memory>
#include <sstream>
#include "uci_move_parse.h"
#include "time_manager.h"
#include "bench_fens.h"
#include "../engine.h"
#include "../utils/fen_constants.h"
#include "../utils/helpers.h"
#include "../move_search/types.h"
#include "../move_search/pvs.h"

void initialize_uci(Position& p);
void uci_position(Position& board, const string& input_line);
void parse_move_time(Color side_to_play, const string& move_time_s, SearchParameters& params);
void uci_go(ThreadData &tdata, Position &board, const string &input_line, SearchParameters &sparams);
void bench();
Position datagen_random_game();
void single_thread_datagen(const string& output_file_path);
void read_uci();
