//
// Created by Archishmaan Peyyety on 2/23/23.
//
#pragma once
#include <algorithm>
#include <utility>
#include "../types.h"

// {Soft Limit, Hard Limit}
using TimeBounds = std::pair<i32, i32>;

TimeBounds allocate_time_moves_to_go(i32 time_remaining, i32 moves_to_go);
TimeBounds allocate_time_standard(i32 time_remaining, i32 increment);
TimeBounds allocate_time(i32 time_remaining, i32 increment, i32 moves_to_go);
