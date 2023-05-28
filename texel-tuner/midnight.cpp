//
// Created by Archishmaan Peyyety on 3/2/23.
//

#include "string"
#include "sstream"
#include "iostream"
#include "cmath"
#include "midnight.h"
#include "../src/board/position.h"
#include "../src/evaluation/evaluate.h"

constexpr int NEVAL_PTYPES = NPIECE_TYPES - 1;

void add_param(parameters_t& params, const Score score) {
	pair_t pair = { (double) mg_score(score), (double) eg_score(score) };
	params.push_back(pair);
}

void add_params(parameters_t& params, const Score scores[], int size) {
	for (int i = 0; i < size; i++) {
		add_param(params, scores[i]);
	}
}

parameters_t Midnight::MidnightEval::get_initial_parameters() {
	parameters_t parameters;

	add_params(parameters, PIECE_VALUES, NEVAL_PTYPES);

	add_params(parameters, PAWN_TABLE, NSQUARES);
	add_params(parameters, KNIGHT_TABLE, NSQUARES);
	add_params(parameters, BISHOP_TABLE, NSQUARES);
	add_params(parameters, ROOK_TABLE, NSQUARES);
	add_params(parameters, QUEEN_TABLE, NSQUARES);
	add_params(parameters, KING_TABLE, NSQUARES);
	add_params(parameters, PASSED_PAWN_BONUS, NSQUARES);
	add_params(parameters, BLOCKED_PASSED_PAWN_PENALTY, NSQUARES);

	add_params(parameters, OPEN_FILE_BONUS, NEVAL_PTYPES);
	add_params(parameters, SEMI_OPEN_FILE_BONUS, NEVAL_PTYPES);
	add_params(parameters, PAWN_PROTECTION, NEVAL_PTYPES);
	add_params(parameters, ATTACKED_BY_PAWN, NEVAL_PTYPES);
	add_params(parameters, THREATS, NEVAL_PTYPES * NEVAL_PTYPES);
	add_params(parameters, KING_RING_ATTACK_BONUS, NEVAL_PTYPES);
	add_params(parameters, CHECK_BONUS, NEVAL_PTYPES);
	add_params(parameters, CENTER_CONTROL, NEVAL_PTYPES);
	add_params(parameters, KING_PAWN_SHIELD, 2);
	add_param(parameters, ISOLATED_PAWN_PENALTY);
	add_param(parameters, DOUBLED_PAWN_PENALTY);
	add_param(parameters, BISHOP_PAIR_BONUS);
	add_params(parameters, KNIGHT_MOBILITY, 9);
	add_params(parameters, BISHOP_MOBILITY, 14);
	add_params(parameters, ROOK_MOBILITY, 15);
	add_params(parameters, QUEEN_MOBILITY, 28);
	add_params(parameters, KING_LINE_SAFETY, 28);

	add_params(parameters, PHALANX_PAWN, NRANKS);
	add_params(parameters, CANDIDATE_PASSED_PAWN, NRANKS);

	add_param(parameters, TEMPO);
	return parameters;
}

EvalResult Midnight::MidnightEval::get_fen_eval_result(const std::string &fen) {
	Position p(fen);
	Trace trace;

	if (p.turn() == BLACK) trace = evaluate<BLACK, EnableTrace>(p);
	else trace = evaluate<WHITE, EnableTrace>(p);

	coefficients_t coefficients;
	get_coefficient_array(coefficients, trace.material, NEVAL_PTYPES);

	get_coefficient_array(coefficients, trace.pawn_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.knight_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.bishop_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.rook_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.queen_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.king_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.passed_pawns, NSQUARES);
	get_coefficient_array(coefficients, trace.blocked_passed_pawns, NSQUARES);

	get_coefficient_array(coefficients, trace.open_files, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.semi_open_files, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.pawn_protection, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.attacked_by_pawn, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.threats, NEVAL_PTYPES * NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.king_ring_bonus, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.check_bonus, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.center_control, NEVAL_PTYPES);
	get_coefficient_array(coefficients, trace.king_pawn_shield, 2);
	get_coefficient_single(coefficients, trace.isolated_pawns);
	get_coefficient_single(coefficients, trace.doubled_pawns);
	get_coefficient_single(coefficients, trace.bishop_bonus);

	get_coefficient_array(coefficients, trace.knight_mobility, 9);
	get_coefficient_array(coefficients, trace.bishop_mobility, 14);
	get_coefficient_array(coefficients, trace.rook_mobility, 15);
	get_coefficient_array(coefficients, trace.queen_mobility, 28);
	get_coefficient_array(coefficients, trace.king_safe_line, 28);

	get_coefficient_array(coefficients, trace.pawn_phalanx, NRANKS);
	get_coefficient_array(coefficients, trace.candidate_pawns, NRANKS);

	get_coefficient_single(coefficients, trace.tempo);
	EvalResult result;
	result.score = trace.score;
	result.coefficients = coefficients;
	return result;
}

static int32_t round_value(tune_t value) {
	return static_cast<int32_t>(round(value));
}

static void print_parameter(std::stringstream& ss, const pair_t parameter) {
	const auto mg = round(parameter[static_cast<int32_t>(PhaseStages::Midgame)]);
	const auto eg = round(parameter[static_cast<int32_t>(PhaseStages::Endgame)]);
	ss << "S(" << mg << ", " << eg << ")";
}

static void print_parameter(std::stringstream& ss, const tune_t parameter) {
	ss << round_value(std::round(parameter));
}

static void print_single(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name) {
	ss << "constexpr Score " << name << " = ";
	print_parameter(ss, parameters[index]);
	index++;

	ss << ";" << std::endl;
}

static void print_array(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {";
	for (auto i = 0; i < count; i++)
	{
		if (i % 8 == 0) ss << "\n\t";
		print_parameter(ss, parameters[index]);
		index++;

		if (i != count - 1)
		{
			ss << ",\t";
		}
	}
	ss << "\n};" << std::endl;
}

static void print_threats(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {";
	for (auto i = 0; i < count; i++)
	{
		if (i % 6 == 0) ss << "\n\t";
		print_parameter(ss, parameters[index]);
		index++;

		if (i != count - 1)
		{
			ss << ",\t";
		}
	}
	ss << "\n};" << std::endl;
}

static void print_pst(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count) {
	ss << "constexpr Score " << name << "[] = {\n";
	for (auto row = 0; row < 8; row++) {
		for (auto j = 0; j < 8; j++) {
			auto i = (row * 8) + j;
			if (j == 0) ss << "\t";
			const auto mg = round(parameters[index][static_cast<int32_t>(PhaseStages::Midgame)]);
			const auto eg = round(parameters[index][static_cast<int32_t>(PhaseStages::Endgame)]);
			ss << "S(" << mg << ", " << eg << ")";
			index++;

			if (i != 64 - 1) {
				ss << ",";
			}
			if (j != 7) {
				ss << "\t";
			}
		}
		if (row != 7) ss << "\n";
	}
	ss << "\n};" << std::endl;
}

static void print_max_material(std::stringstream& ss, const parameters_t& parameters) {
	ss << "const int max_material[] = {";
	for (auto i = 0; i < 6; i++)
	{
		const auto mg = parameters[i][static_cast<int>(PhaseStages::Midgame)];
		const auto eg = parameters[i][static_cast<int>(PhaseStages::Endgame)];
		const auto max_material = round_value(std::max(mg, eg));
		ss << max_material << ", ";
	}
	ss << "0};" << std::endl;
}

void Midnight::MidnightEval::print_parameters(const parameters_t &parameters) {
	parameters_t parameters_copy = parameters;

	int index = 0;
	std::stringstream ss;
	print_array(ss, parameters_copy, index, "PIECE_VALUES", NEVAL_PTYPES);
	print_pst(ss, parameters_copy, index, "PAWN_TABLE", NSQUARES);
	print_pst(ss, parameters_copy, index, "KNIGHT_TABLE", NSQUARES);
	print_pst(ss, parameters_copy, index, "BISHOP_TABLE", NSQUARES);
	print_pst(ss, parameters_copy, index, "ROOK_TABLE", NSQUARES);
	print_pst(ss, parameters_copy, index, "QUEEN_TABLE", NSQUARES);
	print_pst(ss, parameters_copy, index, "KING_TABLE", NSQUARES);
	print_pst(ss, parameters_copy, index, "PASSED_PAWN_BONUS", NSQUARES);
	print_pst(ss, parameters_copy, index, "BLOCKED_PASSED_PAWN_PENALTY", NSQUARES);

	print_array(ss, parameters_copy, index, "OPEN_FILE_BONUS", NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "SEMI_OPEN_FILE_BONUS", NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "PAWN_PROTECTION", NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "ATTACKED_BY_PAWN", NEVAL_PTYPES);
	print_threats(ss, parameters_copy, index, "THREATS", NEVAL_PTYPES * NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "KING_RING_ATTACK_BONUS", NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "CHECK_BONUS", NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "CENTER_CONTROL", NEVAL_PTYPES);
	print_array(ss, parameters_copy, index, "KING_PAWN_SHIELD", 2);
	print_single(ss, parameters_copy, index, "ISOLATED_PAWN_PENALTY");
	print_single(ss, parameters_copy, index, "DOUBLED_PAWN_PENALTY");
	print_single(ss, parameters_copy, index, "BISHOP_PAIR_BONUS");

	print_array(ss, parameters_copy, index, "KNIGHT_MOBILITY", 9);
	print_array(ss, parameters_copy, index, "BISHOP_MOBILITY", 14);
	print_array(ss, parameters_copy, index, "ROOK_MOBILITY", 15);
	print_array(ss, parameters_copy, index, "QUEEN_MOBILITY", 28);
	print_array(ss, parameters_copy, index, "KING_LINE_SAFETY", 28);

	print_array(ss, parameters_copy, index, "PHALANX_PAWN", NRANKS);
	print_array(ss, parameters_copy, index, "CANDIDATE_PASSED_PAWN", NRANKS);

	print_single(ss, parameters_copy, index, "TEMPO");
	std::cout << ss.str() << "\n";
}
