//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once

#include <cstring>
#include <cstdint>
#include "../board/types/piece.h"
#include "../board/types/square.h"

using Score = int;
constexpr Score SCORE_ZERO = 0;

consteval Score S(int mg, int eg) {
	return Score(static_cast<int>(static_cast<unsigned int>(eg) << 16) + mg);
}

inline int eg_value(Score s) {
	auto eg = static_cast<uint16_t>(static_cast<uint32_t>(s + 0x8000) >> 16);

	int16_t v;
	std::memcpy(&v, &eg, sizeof(eg));

	return static_cast<int>(v);
}

inline int mg_value(Score s) {
	auto mg = static_cast<uint16_t>(s);

	int16_t v;
	std::memcpy(&v, &mg, sizeof(mg));

	return static_cast<int>(v);
}

struct Trace {
	int score={};

	short material[NPIECE_TYPES][NCOLORS]{};

	short pawn_pst[NSQUARES][NCOLORS]{};
	short knight_pst[NSQUARES][NCOLORS]{};
	short bishop_pst[NSQUARES][NCOLORS]{};
	short rook_pst[NSQUARES][NCOLORS]{};
	short queen_pst[NSQUARES][NCOLORS]{};
	short king_pst[NSQUARES][NCOLORS]{};

	short open_files[NPIECE_TYPES][NCOLORS]{};
	short semi_open_files[NPIECE_TYPES][NCOLORS]{};
	short pawn_protection[NPIECE_TYPES][NCOLORS]{};
	short attacked_by_pawn[NPIECE_TYPES][NCOLORS]{};
	short threats[NPIECE_TYPES * NPIECE_TYPES][NCOLORS]{};
	short king_ring_bonus[NPIECE_TYPES][NCOLORS]{};
	short check_bonus[NPIECE_TYPES][NCOLORS]{};
	short center_control[NPIECE_TYPES][NCOLORS]{};

	short king_pawn_shield[2][NCOLORS]{};
	short passed_pawns[NSQUARES][NCOLORS]{};
	short blocked_passed_pawns[NSQUARES][NCOLORS]{};
	short isolated_pawns[NCOLORS]{};
	short doubled_pawns[NCOLORS]{};
	short pawnless_king_flank[NCOLORS]{};
	short bishop_bonus[NCOLORS]{};

	short knight_mobility[9][NCOLORS]{};
	short bishop_mobility[14][NCOLORS]{};
	short rook_mobility[15][NCOLORS]{};
	short queen_mobility[28][NCOLORS]{};

	short knight_forward_mobility[9][NCOLORS]{};
	short bishop_forward_mobility[14][NCOLORS]{};
	short rook_forward_mobility[15][NCOLORS]{};
	short queen_forward_mobility[28][NCOLORS]{};

	short king_safe_line[28][NCOLORS]{};

	short pawn_phalanx[NRANKS][NCOLORS]{};
	short candidate_pawns[NRANKS][NCOLORS]{};

	short tempo[NCOLORS]{};
};

using DoTrace = bool;
constexpr DoTrace TRACE_EVAL = true;
constexpr DoTrace COMPUTE_EVAL = false;
using EnableTrace = Trace;
using DisableTrace = int;
