//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once
#include <array>
#include "cstring"
#include "search_params.h"
#include "reductions.h"
#include "pruning.h"
#include "extensions.h"
#include "../evaluation/evaluate.h"
#include "../utils/clock.h"
#include "../board/position.h"
#include "tables/transposition_table.h"
#include "../move_gen/move_generator.h"
#include "move_ordering/move_ordering.h"

extern PVSData data;

void reset_data();
bool position_is_draw(Position &board, i32 ply);
void update_best_move_results(i32 sub_depth, bool debug);
i32 scale_soft_time_limit(BestMoveSearchParameters &params, PVSData& ab_results, i32 depth);

template<Color color>
i32 q_search(ThreadData &tdata, Position &board, i32 ply, i32 alpha, i32 beta);

template<Color color>
i32 pvs(ThreadData &tdata, Position &board, i16 depth, i32 ply, i32 alpha, i32 beta, bool do_null);

template<Color color>
PVSData aspiration_windows(ThreadData &tdata, Position &board, i32 prev_score, i16 depth, i32 time_limit);

template<Color color>
void iterative_deepening(ThreadData &tdata, Position &board, BestMoveSearchParameters &params);

void search(ThreadData &tdata, Position &board, BestMoveSearchParameters &parameters);