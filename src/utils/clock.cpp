//
// Created by Archishmaan Peyyety on 1/31/23.
//

#include "clock.h"

void reset_clock() {
	start_time = std::chrono::high_resolution_clock::now();
}

int get_elapsed_time(TimeResolution resolution) {
	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = current_time - start_time;
	double seconds_elapsed = diff.count();
	return static_cast<i32>(seconds_elapsed * static_cast<i32>(resolution));
}

bool time_elapsed_exceeds(i32 magnitude, TimeResolution resolution) {
	return get_elapsed_time(resolution) >= magnitude;
}
