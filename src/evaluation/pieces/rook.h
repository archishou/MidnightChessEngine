#pragma once
#include "board/position.h"
#include "evaluation/types.h"

template<Color color>
Score evaluate_rooks(const Position& board);