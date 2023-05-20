//
// Created by Archishmaan Peyyety on 2/24/23.
//
#pragma once
#include "evaluation/bitboards.h"
#include "evaluation/types.h"
#include "evaluation/constants/psts.h"
#include "evaluation/constants/misc.h"

template<Color color, DoTrace do_trace>
Score evaluate_pawn_structure(const Position& board, Trace& trace);
