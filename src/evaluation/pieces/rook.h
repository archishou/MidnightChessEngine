#pragma once
#include "board/position.h"
#include "evaluation/types.h"

template<Color color, DoTrace do_trace>
Score evaluate_rooks(const Position& board, Trace& trace);