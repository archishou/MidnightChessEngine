#pragma once
#include "move_search/search_params.h"
#include "move_search/search_constants.h"

extern double lmr_table[MAX_DEPTH + 1][MAX_DEPTH + 1];
extern void init_lmr_table();
