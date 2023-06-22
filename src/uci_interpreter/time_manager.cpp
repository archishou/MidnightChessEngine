//
// Created by Archishmaan Peyyety on 6/10/23.
//
#include "time_manager.h"

TimeBounds allocate_time(i32 time_remaining, i32 increment, i32 moves_to_go) {
	TimeBounds time_bounds{};
	if (moves_to_go == -1) time_bounds = allocate_time_standard(time_remaining, increment);
	else time_bounds = allocate_time_moves_to_go(time_remaining, moves_to_go);
	return time_bounds;
}

TimeBounds allocate_time_standard(i32 time_remaining, i32 increment) {
	const auto soft_limit = (time_remaining / 40) + (3 * increment / 4);
	const auto hard_limit = (time_remaining / 5) + (3 * increment / 4);
	return {soft_limit, hard_limit};
}

TimeBounds allocate_time_moves_to_go(i32 time_remaining, i32 moves_to_go) {
	moves_to_go = std::min(moves_to_go, 50);
	auto scale = 0.7 / moves_to_go;
	auto eight = 0.95 * time_remaining;
	auto optimum = static_cast<i32>(std::min(scale * time_remaining, eight));

	TimeBounds bounds{};
	bounds.second = static_cast<i32>(std::min(5.0 * optimum, eight));
	bounds.first = optimum;
	return bounds;
}

