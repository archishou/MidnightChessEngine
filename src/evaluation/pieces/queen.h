#pragma once

#include "board/types/board_types.h"
#include "evaluation/types.h"
#include "board/position.h"

template<Color color, DoTrace do_trace>
Score evaluate_queens(const Position& board, Trace& trace);
