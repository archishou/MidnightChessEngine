#pragma once

#include "board/types/board_types.h"
#include "evaluation/types.h"
#include "board/position.h"

template<Color color>
Score evaluate_queens(const Position& board);
