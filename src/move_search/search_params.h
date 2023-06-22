//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once
#include "../types.h"

constexpr i32 RFP_MARGIN = 75;
constexpr i32 RFP_MAX_DEPTH = 9;

constexpr double LMR_BASE_CAPTURE = 1.40;
constexpr double LMR_DIVISOR_CAPTURE = 1.80;
constexpr double LMR_BASE_QUIET = 1.50;
constexpr double LMR_DIVISOR_QUIET = 1.75;

constexpr i32 NMP_MIN_DEPTH = 3;

constexpr i32 LMP_MIN_DEPTH = 3;
constexpr i32 LMP_DEPTH_MULTIPLIER = 12;

constexpr i32 Q_SEARCH_FUTILITY_MARGIN = 60;

constexpr i32 ASP_WINDOW_MIN_DEPTH = 6;
constexpr i32 ASP_WINDOW_INIT_WINDOW = 12;
constexpr i32 ASP_WINDOW_INIT_DELTA = 16;
constexpr i32 ASP_WINDOW_FULL_SEARCH_BOUNDS = 3500;

constexpr i32 SEE_PVS_MIN_DEPTH = 7;
constexpr i32 SEE_PVS_QUIET_MARGIN = -50;
constexpr i32 SEE_PVS_TACTICAL_MARGIN = -90;
constexpr i32 SEE_Q_MARGIN = 1;

constexpr i32 FP_COEFFICIENT = 100;
constexpr i32 FP_MARGIN = 75;
constexpr i32 FP_DEPTH = 6;