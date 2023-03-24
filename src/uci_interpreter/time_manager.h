//
// Created by Archishmaan Peyyety on 2/23/23.
//
#pragma once
int time_iterative_deepening(int time_remaining, int increment) {
	return (time_remaining / 10) + (increment / 2);
}

int time_search(int time_remaining, int increment) {
	return (time_remaining / 40) + (increment / 2);
}