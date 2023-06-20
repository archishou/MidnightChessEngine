#pragma once
#include "string"
#include "sstream"
#include "iostream"
#include "cmath"
#include "midnight.h"
#include "../src/board/position.h"
#include "../src/evaluation/evaluate.h"
#include <iomanip>

inline void add_param(parameters_t& params, const Score score) {
	pair_t pair = { (double) mg_score(score), (double) eg_score(score) };
	params.push_back(pair);
}

inline void add_params(parameters_t& params, const Score scores[], int size) {
	for (int i = 0; i < size; i++) {
		add_param(params, scores[i]);
	}
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

inline void print_array(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {\n";
	for (auto i = 0; i < count; i++) {
		if (i % 8 == 0) ss << std::left << std::setw(8) << "";

		const auto mg = round(parameters[index][static_cast<int32_t>(PhaseStages::Midgame)]);
		const auto eg = round(parameters[index][static_cast<int32_t>(PhaseStages::Endgame)]);
		index++;

		std::stringstream score_string;
		score_string << "S(" << mg << ", " << eg << "),";

		if (i % 8 != 7) {
			ss << std::left << std::setw(16) << score_string.str();
		} else if (i != count - 1) {
			ss << score_string.str() << "\n";
		} else ss << score_string.str();

	}
	ss << "\n};" << std::endl;
}

inline void print_threats(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {\n";
	for (auto i = 0; i < count; i++) {
		if (i % 6 == 0) ss << std::left << std::setw(8) << "";

		const auto mg = round(parameters[index][static_cast<int32_t>(PhaseStages::Midgame)]);
		const auto eg = round(parameters[index][static_cast<int32_t>(PhaseStages::Endgame)]);
		index++;

		std::stringstream score_string;
		score_string << "S(" << mg << ", " << eg << "),";

		if (i % 6 != 5) {
			ss << std::left << std::setw(16) << score_string.str();
		} else if (i != count - 1) {
			ss << score_string.str() << "\n";
		} else ss << score_string.str();
	}
	ss << "\n};" << std::endl;
}

inline void print_pst(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {\n";
	for (auto row = 0; row < 8; row++) {
		for (auto j = 0; j < 8; j++) {
			if (j == 0) ss << std::left << std::setw(8) << "";
			const auto mg = round(parameters[index][static_cast<int32_t>(PhaseStages::Midgame)]);
			const auto eg = round(parameters[index][static_cast<int32_t>(PhaseStages::Endgame)]);
			std::stringstream score_string;
			score_string << "S(" << mg << ", " << eg << "),";

			if (j != 7) {
				ss << std::left << std::setw(16) << score_string.str();
			} else ss << score_string.str();

			index++;
		}
		if (row != 7) ss << "\n";
	}
	ss << "\n};" << std::endl;
}

