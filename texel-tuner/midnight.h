#ifndef MIDNIGHT_H
#define MIDNIGHT_H

#include "base.h"
#include <string>
#include <vector>

namespace Midnight
{
	class MidnightEval {
	public:
		constexpr static bool includes_additional_score = true;
		static parameters_t get_initial_parameters();
		static EvalResult get_fen_eval_result(const std::string& fen);
		static void print_parameters(const parameters_t& parameters);
	};
}


#endif // MIDNIGHT_H