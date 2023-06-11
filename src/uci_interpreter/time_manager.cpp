//
// Created by Archishmaan Peyyety on 6/10/23.
//
#include "time_manager.h"

TimeBounds allocate_time(int time_remaining, int increment, int moves_to_go) {
	TimeBounds time_bounds{};
	if (moves_to_go == -1) time_bounds = allocate_time_standard(time_remaining, increment);
	else time_bounds = allocate_time_moves_to_go(time_remaining, moves_to_go);
	return time_bounds;
}

TimeBounds allocate_time_standard(int time_remaining, int increment) {
	TimeBounds bounds{};
	bounds.first = (time_remaining / 40) + (3 * increment / 4);
	bounds.second = (time_remaining / 10) + (3 * increment / 4);
	return bounds;
}

TimeBounds allocate_time_moves_to_go(int time_remaining, int moves_to_go) {
	moves_to_go = std::min(moves_to_go, 50);
	double scale = 0.7 / moves_to_go;
	double eight = 0.95 * time_remaining;
	int optimum = static_cast<int>(std::min(scale * time_remaining, eight));

	TimeBounds bounds{};
	bounds.second = static_cast<int>(std::min(5.0 * optimum, eight));
	bounds.first = optimum;
	return bounds;
}

