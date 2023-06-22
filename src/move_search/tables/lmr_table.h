#pragma once
#include "../search_constants.h"
#include "../search_params.h"

extern array<array<array<double, MAX_DEPTH + 1>, MAX_DEPTH + 1>, 2> lmr_table;
extern void init_lmr_table();
