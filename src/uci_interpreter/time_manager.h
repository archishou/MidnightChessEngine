//
// Created by Archishmaan Peyyety on 2/23/23.
//
#pragma once
inline int time_iterative_deepening(int time_remaining, int increment, int moves_to_go) {
	if (moves_to_go != -1) {
		moves_to_go = std::min(moves_to_go, 50);
		double scale = 0.7 / moves_to_go;
		double eight = 0.95 * time_remaining;
		return static_cast<int>(std::min(scale * time_remaining, eight));
	}
	return (time_remaining / 10) + (increment / 2);
}

inline int time_search(int time_remaining, int increment, int moves_to_go) {
	if (moves_to_go != -1) {
		moves_to_go = std::min(moves_to_go, 50);
		double scale = 0.7 / moves_to_go;
		double eight = 0.95 * time_remaining;
		int opt_time = static_cast<int>(std::min(scale * time_remaining, eight));
		return static_cast<int>(std::min(5.0 * opt_time, eight));
	}
	return (time_remaining / 40) + (increment / 2);
}
