//
// Created by Archishmaan Peyyety on 3/2/23.
//

#include "midnight.h"
#include <fstream>
#include <utility>
#include <tuple>

namespace Midnight {
	using enum EvalOutputs;

	template <bool run_trace = false>
	static auto fetch_texel_features(const string &fen = "") {
		Position p;
		Trace trace{};
		if (!fen.empty()) {
			p.set_fen(fen);
			if (p.turn() == BLACK) trace = evaluate<BLACK, TRACE_EVAL>(p);
			else trace = evaluate<WHITE, TRACE_EVAL>(p);
		}

		std::tuple features = {
			TexelFeatures{"PAWN_TABLE", PSTS, PAWN_TABLE, trace.pawn_pst, NSQUARES},
			TexelFeatures{"KNIGHT_TABLE", PSTS, KNIGHT_TABLE, trace.knight_pst, NSQUARES},
			TexelFeatures{"BISHOP_TABLE", PSTS, BISHOP_TABLE, trace.bishop_pst, NSQUARES},
			TexelFeatures{"ROOK_TABLE", PSTS, ROOK_TABLE, trace.rook_pst, NSQUARES},
			TexelFeatures{"QUEEN_TABLE", PSTS, QUEEN_TABLE, trace.queen_pst, NSQUARES},
			TexelFeatures{"KING_TABLE", PSTS, KING_TABLE, trace.king_pst, NSQUARES},
			TexelFeatures{"PASSED_PAWN_BONUS", PSTS, PASSED_PAWN_BONUS, trace.passed_pawns, NSQUARES},
			TexelFeatures{"BLOCKED_PASSED_PAWN_PENALTY", PSTS, BLOCKED_PASSED_PAWN_PENALTY,
						  trace.blocked_passed_pawns, NSQUARES},

			TexelFeatures{"PIECE_VALUES", MISC, PIECE_VALUES, trace.material, NEVAL_PTYPES},

			TexelFeatures{"OPEN_FILE_BONUS", MISC, OPEN_FILE_BONUS, trace.open_files, NEVAL_PTYPES},
			TexelFeatures{"SEMI_OPEN_FILE_BONUS", MISC, SEMI_OPEN_FILE_BONUS, trace.semi_open_files, NEVAL_PTYPES},
			TexelFeatures{"PAWN_PROTECTION", MISC, PAWN_PROTECTION, trace.pawn_protection, NEVAL_PTYPES},
			TexelFeatures{"ATTACKED_BY_PAWN", MISC, ATTACKED_BY_PAWN, trace.attacked_by_pawn, NEVAL_PTYPES},
			TexelFeatures{"THREATS", MISC, THREATS, trace.threats, NEVAL_PTYPES * NEVAL_PTYPES, NTHREAT_COLS},
			TexelFeatures{"CHECK_BONUS", MISC, CHECK_BONUS, trace.check_bonus, NEVAL_PTYPES},
			TexelFeatures{"CENTER_CONTROL", MISC, CENTER_CONTROL, trace.center_control, NEVAL_PTYPES},
			TexelFeatures{"KING_PAWN_SHIELD", MISC, KING_PAWN_SHIELD, trace.king_pawn_shield, 2},

			TexelFeatures{"KNIGHT_MOBILITY", MISC, KNIGHT_MOBILITY, trace.knight_mobility, NKNIGHT_MOBILITY},
			TexelFeatures{"BISHOP_MOBILITY", MISC, BISHOP_MOBILITY, trace.bishop_mobility, NBISHOP_MOBILITY},
			TexelFeatures{"ROOK_MOBILITY", MISC, ROOK_MOBILITY, trace.rook_mobility, NROOK_MOBILITY},
			TexelFeatures{"QUEEN_MOBILITY", MISC, QUEEN_MOBILITY, trace.queen_mobility, NQUEEN_MOBILITY},

			TexelFeatures{"KNIGHT_FORWARD_MOBILITY", MISC, KNIGHT_FORWARD_MOBILITY, trace.knight_forward_mobility, NKNIGHT_MOBILITY},
			TexelFeatures{"BISHOP_FORWARD_MOBILITY", MISC, BISHOP_FORWARD_MOBILITY, trace.bishop_forward_mobility, NBISHOP_MOBILITY},
			TexelFeatures{"ROOK_FORWARD_MOBILITY", MISC, ROOK_FORWARD_MOBILITY, trace.rook_forward_mobility, NROOK_MOBILITY},
			TexelFeatures{"QUEEN_FORWARD_MOBILITY", MISC, QUEEN_FORWARD_MOBILITY, trace.queen_forward_mobility, NQUEEN_MOBILITY},

			TexelFeatures{"KING_RING_ATTACK_PAWN", MISC, KING_RING_ATTACK_PAWN, trace.king_ring_pawn, NQUEEN_MOBILITY},
			TexelFeatures{"KING_RING_ATTACK_KNIGHT", MISC, KING_RING_ATTACK_KNIGHT, trace.king_ring_knight, NQUEEN_MOBILITY},
			TexelFeatures{"KING_RING_ATTACK_BISHOP", MISC, KING_RING_ATTACK_BISHOP, trace.king_ring_bishop, NQUEEN_MOBILITY},
			TexelFeatures{"KING_RING_ATTACK_ROOK", MISC, KING_RING_ATTACK_ROOK, trace.king_ring_rook, NQUEEN_MOBILITY},
			TexelFeatures{"KING_RING_ATTACK_QUEEN", MISC, KING_RING_ATTACK_QUEEN, trace.king_ring_queen, NQUEEN_MOBILITY},

			TexelFeatures{"KING_LINE_SAFETY", MISC, KING_LINE_SAFETY, trace.king_safe_line, NQUEEN_MOBILITY},

			TexelFeatures{"PHALANX_PAWN", MISC, PHALANX_PAWN, trace.pawn_phalanx, NRANKS},
			TexelFeatures{"CANDIDATE_PASSED_PAWN", MISC, CANDIDATE_PASSED_PAWN, trace.candidate_pawns, NRANKS},

			TexelFeatures{"ISOLATED_PAWN_PENALTY", MISC, ISOLATED_PAWN_PENALTY, trace.isolated_pawns},
			TexelFeatures{"DOUBLED_PAWN_PENALTY", MISC, DOUBLED_PAWN_PENALTY, trace.doubled_pawns},
			TexelFeatures{"BISHOP_PAIR_BONUS", MISC, BISHOP_PAIR_BONUS, trace.bishop_bonus},
			TexelFeatures{"PAWNLESS_KING_FLANK", MISC, PAWNLESS_KING_FLANK, trace.pawnless_king_flank},


			TexelFeatures{"TEMPO", MISC, TEMPO, trace.tempo},
		};

		if constexpr (run_trace) {
			return std::pair {
				features,
				trace.score
			};
		} else {
			return features;
		}
	}

	parameters_t Midnight::MidnightEval::get_initial_parameters() {
		parameters_t parameters;

		auto features = fetch_texel_features();
		std::apply([&parameters](auto&&... args) {
			(push_param(parameters, std::forward<decltype(args)>(args)), ...);
		}, features);

		return parameters;
	}

	EvalResult Midnight::MidnightEval::get_fen_eval_result(const std::string &fen) {

		coefficients_t coefficients;
		auto features = fetch_texel_features<true>(fen);
		std::apply([&coefficients](auto&&... args) {
			(push_coeff(coefficients, std::forward<decltype(args)>(args)), ...);
		}, features.first);

		EvalResult result;
		result.score = features.second;
		result.coefficients = coefficients;
		return result;
	}

	void Midnight::MidnightEval::print_parameters(const parameters_t &parameters) {
		int index = 0;
		auto features = fetch_texel_features();

		std::vector<std::stringstream> outputs{};
		for (usize i = 0; i < NOUTPUTS; i++) {
			outputs.emplace_back();
			print_constant_header(outputs[i]);
		}

		std::apply([&outputs, &parameters, &index](auto&&... args) {
			(print_feature(outputs[static_cast<usize>(args.output)], parameters, index, args), ...);
		}, features);

		for (usize i = 0; i < NOUTPUTS; i++) {
			std::ofstream stream(paths[static_cast<EvalOutputs>(i)], std::ofstream::trunc);
			stream << outputs[i].str() << "\n";
		}
	}
}