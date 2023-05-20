#ifndef CONFIG_H
#define CONFIG_H 1

#include<cstdint>
#include "midnight.h"

#define TAPERED 1
using TuneEval = Midnight::MidnightEval;
constexpr int32_t thread_count = 12;
constexpr double preferred_k = 0;//3.3661;
constexpr bool retune_from_zero = false;

#endif // !CONFIG_H
