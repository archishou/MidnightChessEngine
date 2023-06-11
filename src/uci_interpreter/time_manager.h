//
// Created by Archishmaan Peyyety on 2/23/23.
//
#pragma once
#include <algorithm>
#include <utility>

// {Soft Limit, Hard Limit}
using TimeBounds = std::pair<int, int>;

TimeBounds allocate_time_moves_to_go(int time_remaining, int moves_to_go);
TimeBounds allocate_time_standard(int time_remaining, int increment);
TimeBounds allocate_time(int time_remaining, int increment, int moves_to_go);
