//
// Created by Archishmaan Peyyety on 1/31/23.
//
#pragma once
#include <iostream>
#include <chrono>

static auto start_time = std::chrono::high_resolution_clock::now();
enum class TimeResolution {
	Seconds = 1,
	Milliseconds = 1000,
};

void reset_clock();
int get_elapsed_time(TimeResolution resolution);
bool time_elapsed_exceeds(int magnitude, TimeResolution resolution);
