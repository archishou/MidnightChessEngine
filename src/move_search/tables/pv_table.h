//
// Created by Archishmaan Peyyety on 1/19/23.
//
#pragma once
#include "../search_constants.h"

typedef Move PVTable[MAX_DEPTH][MAX_DEPTH];

struct PV {
	int length[MAX_DEPTH];
	PVTable table;
};

inline void init_pv(PV& pv, int ply) {
	pv.length[ply] = ply;
}

inline void update_pv(PV& pv, int ply, Move best_move) {
	pv.table[ply][ply] = best_move;
	for (int next_ply = ply + 1; next_ply < pv.length[ply + 1]; next_ply++) {
		pv.table[ply][next_ply] = pv.table[ply + 1][next_ply];
	}
	pv.length[ply] = pv.length[ply + 1];
}
