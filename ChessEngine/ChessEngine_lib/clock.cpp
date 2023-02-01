//
// Created by Archishmaan Peyyety on 1/31/23.
//

#include "clock.h"

void reset_clock() {
	start_time = std::clock();
}

int get_elapsed_time(TimeResolution resolution) {
	double time_seconds = (std::clock() - start_time) / (double) CLOCKS_PER_SEC;
	return int(time_seconds * resolution);
}

bool time_elapsed_exceeds(TimeResolution resolution, int magnitude) {
	return get_elapsed_time(resolution) > magnitude;
}
