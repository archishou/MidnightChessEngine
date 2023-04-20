//
// Created by Archishmaan Peyyety on 1/1/23.
//
#pragma once
const int RFP_MARGIN = 75;
const int RFP_MAX_DEPTH = 9;

const double LMR_BASE = 1.2;
const double LMR_DIVISOR = 1.8;

const int NMP_MIN_DEPTH = 3;

const int LMP_MIN_DEPTH = 3;
const int LMP_DEPTH_MULTIPLIER = 12;

constexpr int Q_SEARCH_FUTILITY_MARGIN = 60;

constexpr int ASP_WINDOW_MIN_DEPTH = 6;
constexpr int ASP_WINDOW_INIT_WINDOW = 12;
constexpr int ASP_WINDOW_INIT_DELTA = 16;
constexpr int ASP_WINDOW_FULL_SEARCH_BOUNDS = 3500;

constexpr int SEE_PVS_MIN_DEPTH = 7;
constexpr int SEE_PVS_QUIET_MARGIN = -50;
constexpr int SEE_PVS_TACTICAL_MARGIN = -90;
constexpr int SEE_Q_MARGIN = 1;
