//
// Created by Archishmaan Peyyety on 4/15/23.
//
#include "types.h"
#include "iostream"
#include "../move_gen/types/move.h"

std::ostream& operator<<(std::ostream& os, const Line& line) {
	for(const Move & i : line) {
		if (i == Move()) break;
		os << i << " ";
	}
	return os;
}
