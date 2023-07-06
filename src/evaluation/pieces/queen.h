#pragma once

#include "../bitboards.h"
#include "../../board/position.h"
#include "../constants/constants.h"

template<Color color, DoTrace do_trace>
Score evaluate_queens(const Position &board, const SharedEvalFeatures &eval_features, Trace &trace);
