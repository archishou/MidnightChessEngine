//
// Created by Archishmaan Peyyety on 1/9/23.
//
#pragma once
#include "../engine.h"
#include "uci_move_parse.h"
#include "time_manager.h"
#include "sstream"
#include <tuple>
#include "bench_fens.h"
#include "../utils/fen_constants.h"
#include "../utils/helpers.h"
#include "../move_search/types.h"
#include "../move_search/pvs.h"

struct ReadUCIParameters {
	bool debug_info = true;
};

void initialize_uci(Position& p);
void uci_position(Position& board, const string& input_line);
void parse_move_time(Color side_to_play, const string& move_time_s, BestMoveSearchParameters& params);
void uci_go(Position& board, const string& input_line, ReadUCIParameters& uci_parameters);
void bench();
void read_uci();
