//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once
#include "../types.h"

constexpr auto RFP_MARGIN = 75;
constexpr auto RFP_MAX_DEPTH = 9;

constexpr auto LMR_BASE_CAPTURE = 1.40;
constexpr auto LMR_DIVISOR_CAPTURE = 1.80;
constexpr auto LMR_BASE_QUIET = 1.50;
constexpr auto LMR_DIVISOR_QUIET = 1.75;

constexpr auto NMP_MIN_DEPTH = 3;

constexpr auto LMP_MIN_DEPTH = 3;
constexpr auto LMP_DEPTH_MULTIPLIER = 12;

constexpr auto Q_SEARCH_FUTILITY_MARGIN = 60;

constexpr auto ASP_WINDOW_MIN_DEPTH = 6;
constexpr auto ASP_WINDOW_INIT_WINDOW = 12;
constexpr auto ASP_WINDOW_INIT_DELTA = 16;
constexpr auto ASP_WINDOW_FULL_SEARCH_BOUNDS = 3500;

constexpr auto SEE_PVS_MIN_DEPTH = 7;
constexpr auto SEE_PVS_QUIET_MARGIN = -50;
constexpr auto SEE_PVS_TACTICAL_MARGIN = -90;
constexpr auto SEE_Q_MARGIN = 1;

constexpr auto FP_COEFFICIENT = 100;
constexpr auto FP_MARGIN = 75;
constexpr auto FP_DEPTH = 6;