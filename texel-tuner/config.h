#pragma once
#include<cstdint>
#include "midnight.h"

using TuneEval = Midnight::MidnightEval;
constexpr int32_t thread_count = 12;
constexpr double preferred_k = 0;//3.3661;
constexpr bool retune_from_zero = false;
