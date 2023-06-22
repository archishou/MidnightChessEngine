#pragma once


#include "../move_gen/types/move.h"

i32 lmr_reduction(bool pv_node, i32 ply, bool in_check, bool improving, i32 move_idx,
				  i32 depth, Move legal_move);
i32 nmp_reduction(i32 depth, i32 beta, i32 static_eval);
