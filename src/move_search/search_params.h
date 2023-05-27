//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once

constexpr int RFP_MARGIN = 75;
constexpr int RFP_MAX_DEPTH = 9;

constexpr double LMR_BASE_CAPTURE = 1.40;
constexpr double LMR_DIVISOR_CAPTURE = 1.80;
constexpr double LMR_BASE_QUIET = 1.50;
constexpr double LMR_DIVISOR_QUIET = 1.75;

constexpr int NMP_MIN_DEPTH = 3;

constexpr int LMP_MIN_DEPTH = 3;
constexpr int LMP_DEPTH_MULTIPLIER = 12;

constexpr int Q_SEARCH_FUTILITY_MARGIN = 60;

constexpr int ASP_WINDOW_MIN_DEPTH = 6;
constexpr int ASP_WINDOW_INIT_WINDOW = 12;
constexpr int ASP_WINDOW_INIT_DELTA = 16;
constexpr int ASP_WINDOW_FULL_SEARCH_BOUNDS = 3500;

constexpr int SEE_PVS_MIN_DEPTH = 7;
constexpr int SEE_PVS_QUIET_MARGIN = -50;
constexpr int SEE_PVS_TACTICAL_MARGIN = -90;
constexpr int SEE_Q_MARGIN = 1;

constexpr int FP_COEFFICIENT = 100;
constexpr int FP_MARGIN = 75;
constexpr int FP_DEPTH = 6;