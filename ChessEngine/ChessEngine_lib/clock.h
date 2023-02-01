//
// Created by Archishmaan Peyyety on 1/31/23.
//

#include <iostream>

std::clock_t start_time = std::clock();
enum TimeResolution {
	Seconds = 1,
	Milliseconds = 1000,
};

void reset_clock();
int get_elapsed_time(TimeResolution resolution);
bool time_elapsed_exceeds(TimeResolution resolution, int magnitude);