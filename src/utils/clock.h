//
// Created by Archishmaan Peyyety on 1/31/23.
//
#pragma once
#include <iostream>
#include <chrono>
#include "../types.h"

static auto start_time = std::chrono::high_resolution_clock::now();
enum class TimeResolution {
	Seconds = 1,
	Milliseconds = 1000,
};

void reset_clock();
i32 get_elapsed_time(TimeResolution resolution);
bool time_elapsed_exceeds(i32 magnitude, TimeResolution resolution);
