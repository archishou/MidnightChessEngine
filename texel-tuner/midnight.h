#pragma once
#include "base.h"
#include <string>
#include <vector>
#include "string"
#include "sstream"
#include "iostream"
#include "cmath"
#include "midnight.h"
#include "../src/board/position.h"
#include "../src/evaluation/evaluate.h"
#include <iomanip>

namespace Midnight {

	constexpr int NOUTPUTS = 2;
	enum class EvalOutputs {
		PSTS = 0,
		MISC = 1
	};

	static std::unordered_map<EvalOutputs, std::string> paths = {
		{EvalOutputs::PSTS, "/Users/archishmaan/Documents/CodeProjects/chess-engine/src/evaluation/constants/psts.h"},
		{EvalOutputs::MISC, "/Users/archishmaan/Documents/CodeProjects/chess-engine/src/evaluation/constants/misc.h"},
	};

	template<typename T, typename U>
	struct TexelFeatures {
		string name;
		EvalOutputs output;
		U initial_params;
		T& trace;
		int size;
		int ncols;

		explicit TexelFeatures(string name_, EvalOutputs outputs_, U params_, T& trace_, int size_ = 1, int ncols_ = 8):
			name(std::move(name_)), output(outputs_), initial_params(params_), trace(trace_), size(size_), ncols(ncols_) {}
		TexelFeatures() = delete;
	};

	inline void add_param(parameters_t& params, const Score score) {
		pair_t pair = { (double) mg_score(score), (double) eg_score(score) };
		params.push_back(pair);
	}

	inline void add_params(parameters_t& params, const Score scores[], int size) {
		for (int i = 0; i < size; i++) {
			add_param(params, scores[i]);
		}
	}

	template <typename T>
	void push_param(parameters_t& params, T&& arg){
		if constexpr(std::is_pointer_v<decltype(arg.initial_params)>) add_params(params, arg.initial_params, arg.size);
		else add_param(params, arg.initial_params);
	}

	template <typename T>
	void push_coeff(coefficients_t& coeffs, T&& arg){
		if constexpr(std::is_pointer_v<decltype(arg.initial_params)>) get_coefficient_array(coeffs, arg.trace, arg.size);
		else get_coefficient_single(coeffs, arg.trace);
	}

	inline void print_constant_header(std::stringstream& ss) {
		ss << "#pragma once\n";

		ss << "#include \"../../board/types/piece.h\"\n";
		ss << "#include \"../types.h\"\n";
	}

	inline void print_parameter(std::stringstream& ss, const pair_t parameter) {
		const auto mg = round(parameter[static_cast<int32_t>(PhaseStages::Midgame)]);
		const auto eg = round(parameter[static_cast<int32_t>(PhaseStages::Endgame)]);
		ss << "S(" << mg << ", " << eg << ")";
	}

	inline void print_single(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name) {
		ss << "constexpr Score " << name << " = ";
		print_parameter(ss, parameters[index]);
		index++;

		ss << ";" << std::endl;
	}

	inline void print_rows(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count, int ncols = 8) {
		ss << "constexpr Score " << name << "[] = {\n";
		for (auto i = 0; i < count; i++) {
			if (i % ncols == 0) ss << std::left << std::setw(8) << "";

			const auto mg = round(parameters[index][static_cast<int32_t>(PhaseStages::Midgame)]);
			const auto eg = round(parameters[index][static_cast<int32_t>(PhaseStages::Endgame)]);
			index++;

			std::stringstream score_string;
			score_string << "S(" << mg << ", " << eg << "),";

			if (i % ncols != ncols - 1) {
				ss << std::left << std::setw(16) << score_string.str();
			} else if (i != count - 1) {
				ss << score_string.str() << "\n";
			} else ss << score_string.str();
		}
		ss << "\n};" << std::endl;
	}

	template <typename T>
	void print_feature(std::stringstream& ss, const parameters_t &parameters, int& index, T&& arg){
		if constexpr(std::is_pointer_v<decltype(arg.initial_params)>) {
			print_rows(ss, parameters, index, arg.name, arg.size, arg.ncols);
		} else print_single(ss, parameters, index, arg.name);
	}

	constexpr int NEVAL_PTYPES = NPIECE_TYPES - 1;
	constexpr int NKNIGHT_MOBILITY = 9;
	constexpr int NBISHOP_MOBILITY = 14;
	constexpr int NROOK_MOBILITY = 15;
	constexpr int NQUEEN_MOBILITY = 28;
	constexpr int NTHREAT_COLS = 6;

	class MidnightEval {
	public:
		constexpr static bool includes_additional_score = false;
		static parameters_t get_initial_parameters();
		static EvalResult get_fen_eval_result(const std::string& fen);
		static void print_parameters(const parameters_t& parameters);
	};
}

