//
// Created by Archishmaan Peyyety on 2/24/23.
//
#pragma once
#include "../bitboards.h"
#include "../../board/position.h"
#include "../constants/constants.h"

template<Color color, DoTrace do_trace>
Score evaluate_un_cachable_pawn_struct(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);

template<Color color, DoTrace do_trace>
Score evaluate_cachable_pawn_struct(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
