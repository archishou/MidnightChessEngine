//
// Created by Archishmaan Peyyety on 3/2/23.
//

#include "helper.h"
#include <fstream>

constexpr int NEVAL_PTYPES = NPIECE_TYPES - 1;

parameters_t Midnight::MidnightEval::get_initial_parameters() {
	parameters_t parameters;

	add_params(parameters, PAWN_TABLE, NSQUARES);
	add_params(parameters, KNIGHT_TABLE, NSQUARES);
	add_params(parameters, BISHOP_TABLE, NSQUARES);
	add_params(parameters, ROOK_TABLE, NSQUARES);
	add_params(parameters, QUEEN_TABLE, NSQUARES);
	add_params(parameters, KING_TABLE, NSQUARES);
	add_params(parameters, PASSED_PAWN_BONUS, NSQUARES);
	add_params(parameters, BLOCKED_PASSED_PAWN_PENALTY, NSQUARES);

	add_params(parameters, PIECE_VALUES, NEVAL_PTYPES);

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

	add_params(parameters, KNIGHT_FORWARD_MOBILITY, 9);
	add_params(parameters, BISHOP_FORWARD_MOBILITY, 14);
	add_params(parameters, ROOK_FORWARD_MOBILITY, 15);
	add_params(parameters, QUEEN_FORWARD_MOBILITY, 28);

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
	get_coefficient_array(coefficients, trace.pawn_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.knight_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.bishop_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.rook_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.queen_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.king_pst, NSQUARES);
	get_coefficient_array(coefficients, trace.passed_pawns, NSQUARES);
	get_coefficient_array(coefficients, trace.blocked_passed_pawns, NSQUARES);

	get_coefficient_array(coefficients, trace.material, NEVAL_PTYPES);

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

	get_coefficient_array(coefficients, trace.knight_forward_mobility, 9);
	get_coefficient_array(coefficients, trace.bishop_forward_mobility, 14);
	get_coefficient_array(coefficients, trace.rook_forward_mobility, 15);
	get_coefficient_array(coefficients, trace.queen_forward_mobility, 28);

	get_coefficient_array(coefficients, trace.king_safe_line, 28);

	get_coefficient_array(coefficients, trace.pawn_phalanx, NRANKS);
	get_coefficient_array(coefficients, trace.candidate_pawns, NRANKS);

	get_coefficient_single(coefficients, trace.tempo);

	EvalResult result;
	result.score = trace.score;
	result.coefficients = coefficients;
	return result;
}

void Midnight::MidnightEval::print_parameters(const parameters_t &parameters) {
	int index = 0;
	std::stringstream psts_ss, misc_ss;
	print_constant_header(psts_ss);
	print_constant_header(misc_ss);

	print_pst(psts_ss, parameters, index, "PAWN_TABLE", NSQUARES);
	print_pst(psts_ss, parameters, index, "KNIGHT_TABLE", NSQUARES);
	print_pst(psts_ss, parameters, index, "BISHOP_TABLE", NSQUARES);
	print_pst(psts_ss, parameters, index, "ROOK_TABLE", NSQUARES);
	print_pst(psts_ss, parameters, index, "QUEEN_TABLE", NSQUARES);
	print_pst(psts_ss, parameters, index, "KING_TABLE", NSQUARES);
	print_pst(psts_ss, parameters, index, "PASSED_PAWN_BONUS", NSQUARES);
	print_pst(psts_ss, parameters, index, "BLOCKED_PASSED_PAWN_PENALTY", NSQUARES);

	print_array(misc_ss, parameters, index, "PIECE_VALUES", NEVAL_PTYPES);

	print_array(misc_ss, parameters, index, "OPEN_FILE_BONUS", NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "SEMI_OPEN_FILE_BONUS", NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "PAWN_PROTECTION", NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "ATTACKED_BY_PAWN", NEVAL_PTYPES);
	print_threats(misc_ss, parameters, index, "THREATS", NEVAL_PTYPES * NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "KING_RING_ATTACK_BONUS", NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "CHECK_BONUS", NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "CENTER_CONTROL", NEVAL_PTYPES);
	print_array(misc_ss, parameters, index, "KING_PAWN_SHIELD", 2);
	print_single(misc_ss, parameters, index, "ISOLATED_PAWN_PENALTY");
	print_single(misc_ss, parameters, index, "DOUBLED_PAWN_PENALTY");
	print_single(misc_ss, parameters, index, "BISHOP_PAIR_BONUS");

	print_array(misc_ss, parameters, index, "KNIGHT_MOBILITY", 9);
	print_array(misc_ss, parameters, index, "BISHOP_MOBILITY", 14);
	print_array(misc_ss, parameters, index, "ROOK_MOBILITY", 15);
	print_array(misc_ss, parameters, index, "QUEEN_MOBILITY", 28);

	print_array(misc_ss, parameters, index, "KNIGHT_FORWARD_MOBILITY", 9);
	print_array(misc_ss, parameters, index, "BISHOP_FORWARD_MOBILITY", 14);
	print_array(misc_ss, parameters, index, "ROOK_FORWARD_MOBILITY", 15);
	print_array(misc_ss, parameters, index, "QUEEN_FORWARD_MOBILITY", 28);

	print_array(misc_ss, parameters, index, "KING_LINE_SAFETY", 28);

	print_array(misc_ss, parameters, index, "PHALANX_PAWN", NRANKS);
	print_array(misc_ss, parameters, index, "CANDIDATE_PASSED_PAWN", NRANKS);

	print_single(misc_ss, parameters, index, "TEMPO");

	std::ofstream psts("/Users/archishmaan/Documents/CodeProjects/chess-engine/src/evaluation/constants/psts.h", std::ofstream::trunc);
	std::ofstream misc("/Users/archishmaan/Documents/CodeProjects/chess-engine/src/evaluation/constants/misc.h", std::ofstream::trunc);

	psts << psts_ss.str() << "\n";
	misc << misc_ss.str() << "\n";
}
