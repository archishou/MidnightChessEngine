//
// Created by Archishmaan Peyyety on 3/20/23.
//
#include "engine.h"
#include "move_search/move_ordering/move_ordering.h"
#include "move_search/tables/lmr_table.h"

void initialize_engine() {
	init_lmr_table();
	t_table.reset_table();
}