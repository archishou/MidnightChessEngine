#pragma once

#include "../bitboards.h"
#include "../../board/position.h"
#include "../constants/misc.h"
#include "../constants/psts.h"

template<Color color, DoTrace do_trace>
Score evaluate_bishops(const Position &board, Trace &trace);