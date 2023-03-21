//
// Created by Archishmaan Peyyety on 2/23/23.
//
#pragma once
int time_for_move(int time_remaining, int increment) {
	return (time_remaining / 20) + (increment / 2);
}