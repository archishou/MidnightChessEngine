//
// Created by Archishmaan Peyyety on 3/20/23.
//
#include "engine.h"

void initialize_engine() {
	initialize_move_sort_tables();
	init_lmr_table();
	t_table.reset_table();
}