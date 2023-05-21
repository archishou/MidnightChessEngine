#pragma once

#include "types/types.h"
#include "../board/position.h"
#include "../utils/stack.h"
#include "tables/attack_tables.h"
#include "tables/square_tables.h"
#include "move_gen_masks.h"
#include "iostream"
#include <tuple>

template<Color color, MoveGenerationType move_gen_type = ALL>
class MoveList {
private:
	Stack<Move, 218> move_list{};
	Position& board_;

	struct SharedData {
		Bitboard us_occupancy, them_occupancy, all;
		Square them_king_square, us_king_square;
		Bitboard us_king, them_king;
		Bitboard us_ortho_sliders, us_diag_sliders;
		Bitboard them_ortho_sliders, them_diag_sliders;
		explicit SharedData(Position& board) {
			us_occupancy = board.occupancy<color>();
			them_occupancy = board.occupancy<~color>();
			all = us_occupancy | them_occupancy;

			us_king = board.occupancy<color, KING>();
			them_king = board.occupancy<~color, KING>();
			us_king_square = lsb(us_king);
			them_king_square = lsb(them_king);

			us_diag_sliders = board.diagonal_sliders<color>();
			them_diag_sliders = board.diagonal_sliders<~color>();

			us_ortho_sliders = board.orthogonal_sliders<color>();
			them_ortho_sliders = board.orthogonal_sliders<~color>();
		}
	};

	template<MoveType move_type>
	void push(Square from, Bitboard to) {
		while (to) {
			Square to_square = pop_lsb(to);
			if constexpr (static_cast<bool>(move_type & PROMOTION_TYPE)) {
				constexpr MoveType is_capture = move_type & CAPTURE_TYPE;
				move_list.push(Move(from, to_square, PR_KNIGHT | is_capture));
				move_list.push(Move(from, to_square, PR_BISHOP | is_capture));
				move_list.push(Move(from, to_square, PR_ROOK | is_capture));
				move_list.push(Move(from, to_square, PR_QUEEN | is_capture));
			} else {
				move_list.push(Move(from, to_square, move_type));
			}
		}
	}

	template<MoveType move_type>
	void push_single(Square from, Square to) {
		if constexpr (static_cast<bool>(move_type & PROMOTION_TYPE)) {
			constexpr MoveType is_capture = move_type & CAPTURE_TYPE;
			move_list.push(Move(from, to, PR_KNIGHT | is_capture));
			move_list.push(Move(from, to, PR_BISHOP | is_capture));
			move_list.push(Move(from, to, PR_ROOK | is_capture));
			move_list.push(Move(from, to, PR_QUEEN | is_capture));
		} else {
			move_list.push(Move(from, to, move_type));
		}
	}

	constexpr Bitboard generate_danger(const SharedData& data);
	constexpr std::pair<Bitboard, Bitboard> generate_checkers_and_pinned(const SharedData& data);

	constexpr void push_check_evasions(const SharedData& data, Bitboard danger);
	constexpr bool push_pawn_knight_check_captures(const SharedData& data, Bitboard checker, Bitboard pinned);
	constexpr void push_en_passant(const SharedData& data, Bitboard pinned);
	constexpr void push_castle(const SharedData& data, Bitboard danger);
	constexpr void push_pinned(const SharedData& data, Bitboard pinned, Bitboard quiet_mask, Bitboard capture_mask);
	constexpr void push_non_pinned_pieces(const SharedData& data, Bitboard pinned, Bitboard quiet_mask, Bitboard capture_mask);
	constexpr void push_non_pinned_pawns(const SharedData& data, Bitboard pinned, Bitboard quiet_mask, Bitboard capture_mask);
	constexpr void push_promotions(Bitboard pinned, Bitboard quiet_mask, Bitboard capture_mask);
public:
	explicit MoveList(Position& board);

	[[nodiscard]] inline auto begin() const { return move_list.begin(); }
	[[nodiscard]] inline auto end() const { return move_list.end(); }
	[[nodiscard]] inline auto size() const { return move_list.size(); }

};

template<Color color, MoveGenerationType move_gen_type>
constexpr void
MoveList<color, move_gen_type>::push_promotions(Bitboard pinned, Bitboard quiet_mask, Bitboard capture_mask) {
	Bitboard promotion_candidates = board_.occupancy<color, PAWN>() & ~pinned & MASK_RANK[relative_rank<color>(RANK7)];
	if (!promotion_candidates) return;

	Bitboard west_promo_capture = shift_relative<color, NORTH_WEST>(promotion_candidates) & capture_mask;
	Bitboard east_promo_capture = shift_relative<color, NORTH_EAST>(promotion_candidates) & capture_mask;

	while (west_promo_capture) {
		Square s = pop_lsb(west_promo_capture);
		push_single<PROMOTION_TYPE | CAPTURE_TYPE>(s - relative_dir<color, NORTH_WEST>(), s);
	}

	while (east_promo_capture) {
		Square s = pop_lsb(east_promo_capture);
		push_single<PROMOTION_TYPE | CAPTURE_TYPE>(s - relative_dir<color, NORTH_EAST>(), s);
	}

	if constexpr (move_gen_type == CAPTURES) return;

	Bitboard quiet_promos = shift_relative<color, NORTH>(promotion_candidates) & quiet_mask;
	while (quiet_promos) {
		Square s = pop_lsb(quiet_promos);
		push_single<PROMOTION_TYPE>(s - relative_dir<color, NORTH>(), s);
	}
}

template<Color color, MoveGenerationType move_gen_type>
constexpr void MoveList<color, move_gen_type>::push_check_evasions(const MoveList::SharedData &data, Bitboard danger) {
	Bitboard evasions = tables::attacks<KING>(data.us_king_square, data.all) & ~(data.us_occupancy | danger);
	if constexpr (move_gen_type == ALL) push<QUIET>(data.us_king_square, evasions & ~data.them_occupancy);
	push<CAPTURE_TYPE>(data.us_king_square, evasions & data.them_occupancy);
}

template<Color color, MoveGenerationType move_gen_type>
constexpr std::pair<Bitboard, Bitboard>
MoveList<color, move_gen_type>::generate_checkers_and_pinned(const MoveList::SharedData &data) {
	Bitboard checkers{}, pinned{};

	checkers = (tables::attacks<KNIGHT>(data.us_king_square, data.all) & board_.occupancy<~color, KNIGHT>()) |
			   (tables::attacks<PAWN, color>(data.us_king_square) & board_.occupancy<~color, PAWN>());

	Bitboard candidates = (tables::attacks<ROOK>(data.us_king_square, data.them_occupancy) & data.them_ortho_sliders) |
						  (tables::attacks<BISHOP>(data.us_king_square, data.them_occupancy) & data.them_diag_sliders);

	while (candidates) {
		Square candidate_square = pop_lsb(candidates);
		Bitboard potentially_pinned = tables::square_in_between(data.us_king_square, candidate_square) & data.us_occupancy;
		if (potentially_pinned == 0) checkers ^= square_to_bitboard(candidate_square);
		else if (pop_count(potentially_pinned) == 1) pinned ^= potentially_pinned;
	}
	return {checkers, pinned};
}

template<Color color, MoveGenerationType move_gen_type>
constexpr Bitboard MoveList<color, move_gen_type>::generate_danger(const MoveList::SharedData& data) {
	Bitboard danger = board_.occupancy<~color, PAWN>();
	danger = shift_relative<~color, NORTH_WEST>(danger) | shift_relative<~color, NORTH_EAST>(danger);

	danger |= tables::attacks<KING>(data.them_king_square, data.all);

	Bitboard them_knights = board_.occupancy<~color, KNIGHT>();
	while (them_knights) {
		danger |= tables::attacks<KNIGHT>(pop_lsb(them_knights), data.all);
	}

	Bitboard them_diag_sliders_ = data.them_diag_sliders;
	while (them_diag_sliders_) {
		danger |= tables::attacks<BISHOP>(pop_lsb(them_diag_sliders_), data.all ^ data.us_king);
	}

	Bitboard them_ortho_sliders_ = data.them_ortho_sliders;
	while (them_ortho_sliders_) {
		danger |= tables::attacks<ROOK>(pop_lsb(them_ortho_sliders_), data.all ^ data.us_king);
	}

	return danger;
}

template<Color color, MoveGenerationType move_gen_type>
constexpr void MoveList<color, move_gen_type>::push_pinned(const MoveList::SharedData &data, Bitboard pinned,
														   Bitboard quiet_mask, Bitboard capture_mask) {
	Bitboard pinned_pieces = pinned & ~board_.occupancy<color, KNIGHT>() & ~board_.occupancy<color, PAWN>();
	Bitboard pinned_pawns = pinned & board_.occupancy<color, PAWN>();

	while (pinned_pieces) {
		const Square s = pop_lsb(pinned_pieces);
		Bitboard pinned_to = tables::attacks(type_of(board_.piece_at(s)), s, data.all) & tables::line_of(data.us_king_square, s);

		if constexpr (move_gen_type == ALL) push<QUIET>(s, pinned_to & quiet_mask);
		push<CAPTURE_TYPE>(s, pinned_to & capture_mask);
	}

	while (pinned_pawns) {
		const Square s = pop_lsb(pinned_pawns);
		const Bitboard line_of_s_king = tables::line_of(data.us_king_square, s);

		if (rank_of(s) == relative_rank<color>(RANK7)) {
			Bitboard pinned_pawn_to = tables::attacks<PAWN, color>(s) & capture_mask & line_of_s_king;
			push<PROMOTION_TYPE | CAPTURE_TYPE>(s, pinned_pawn_to);
			continue;
		}
		Bitboard pinned_pawn_to = tables::attacks<PAWN, color>(s) & data.them_occupancy & line_of_s_king;
		push<CAPTURE_TYPE>(s, pinned_pawn_to);

		if constexpr (move_gen_type == CAPTURES) continue;

		pinned_pawn_to = shift_relative<color, NORTH>(square_to_bitboard(s)) & ~data.all & line_of_s_king;

		Bitboard double_push_to = pinned_pawn_to & MASK_RANK[relative_rank<color>(RANK3)];

		Bitboard pinned_pawn_double_push = shift_relative<color, NORTH>(double_push_to) & ~data.all & line_of_s_king;
		push<QUIET>(s, pinned_pawn_to);
		push<DOUBLE_PUSH>(s, pinned_pawn_double_push);
	}
}

template<Color color, MoveGenerationType move_gen_type>
constexpr void MoveList<color, move_gen_type>::push_castle(const MoveList::SharedData &data, Bitboard danger) {
	if constexpr (move_gen_type == CAPTURES) return;

	Bitboard oo_path_in_danger = (data.all | danger) & oo_blockers_mask<color>();

	if (board_.king_and_oo_rook_not_moved<color>() && !oo_path_in_danger) {
		if constexpr (color == WHITE) push_single<OO>(e1, g1);
		else push_single<OO>(e8, g8);
	}

	Bitboard ooo_path_in_danger = (data.all | (danger & ooo_danger_mask<color>())) & ooo_blockers_mask<color>();

	if (board_.king_and_ooo_rook_not_moved<color>() && !ooo_path_in_danger) {
		if constexpr (color == WHITE) push_single<OOO>(e1, c1);
		else push_single<OOO>(e8, c8);
	}
}

template<Color color, MoveGenerationType move_gen_type>
constexpr void MoveList<color, move_gen_type>::push_en_passant(const MoveList::SharedData &data, Bitboard pinned) {
	if (board_.ep_square() == NO_SQUARE) return;

	const Bitboard ep_attackers = tables::attacks<PAWN, ~color>(board_.ep_square()) & board_.occupancy<color, PAWN>();

	Bitboard unpinned_ep_attackers = ep_attackers & ~pinned;

	while (unpinned_ep_attackers) {
		const Square s = pop_lsb(unpinned_ep_attackers);

		const Bitboard ep_captured_pawn = shift_relative<color, SOUTH>(square_to_bitboard(board_.ep_square()));
		const Bitboard masked_all = data.all ^ square_to_bitboard(s) ^ ep_captured_pawn;
		const Bitboard attacking_ortho_sliders = tables::attacks<ROOK>(data.us_king_square, masked_all);

		const Bitboard pseudo_pinned_attackers = attacking_ortho_sliders & MASK_RANK[rank_of(data.us_king_square)] & data.them_ortho_sliders;
		if (pseudo_pinned_attackers != 0) continue;

		push_single<ENPASSANT>(s, board_.ep_square());
	}

	const Bitboard diagonal_pinned_ep_attackers = ep_attackers & pinned & tables::line_of(board_.ep_square(), data.us_king_square);
	if (diagonal_pinned_ep_attackers) {
		push_single<ENPASSANT>(lsb(diagonal_pinned_ep_attackers), board_.ep_square());
	}
}

template<Color color, MoveGenerationType move_gen_type>
constexpr bool MoveList<color, move_gen_type>::push_pawn_knight_check_captures(const MoveList::SharedData &data, Bitboard checker, Bitboard pinned) {
	Square checker_square = lsb(checker);

	Bitboard ep_checker_captures, attacking_checker;
	Square epsq = board_.ep_square();

	// For pawn and knight checks we must capture or evade, we cannot block them.
	switch (board_.piece_at(checker_square)) {
		case make_piece<~color, PAWN>():
			// The checker was a double pushed pawn
			if (checker == shift_relative<color, SOUTH>(square_to_bitboard(epsq))) {
				// We can ep capture the double pushed pawn as it is not pinned.
				ep_checker_captures = tables::attacks<PAWN, ~color>(epsq) & board_.occupancy<color, PAWN>() & ~pinned;
				while (ep_checker_captures) {
					push<ENPASSANT>(pop_lsb(ep_checker_captures), square_to_bitboard(epsq));
				}
			}
			[[fallthrough]];

		case make_piece<~color, KNIGHT>():
			// Checker was a pawn or knight, we must capture (evasions assumed to be handled already)
			attacking_checker = board_.attackers_of<color>(checker_square, data.all) & ~pinned;
			while (attacking_checker) {
				Square s = pop_lsb(attacking_checker);
				// If they promoted to a knight, and we can capture and promote, do that.
				if (type_of(board_.piece_at(s)) == PAWN && rank_of(s) == relative_rank<color>(RANK7)) {
					push<PROMOTION_TYPE | CAPTURE_TYPE>(s, checker);
				}
				else push<CAPTURE_TYPE>(s, checker);
			}
			return true;
		default: return false;
	}
}

template<Color color, MoveGenerationType move_gen_type>
constexpr void MoveList<color, move_gen_type>::push_non_pinned_pieces(const MoveList::SharedData &data, Bitboard pinned,
																	  Bitboard quiet_mask, Bitboard capture_mask) {
	Bitboard un_pinned_knights = board_.occupancy<color, KNIGHT>() & ~pinned;
	while (un_pinned_knights) {
		Square s = pop_lsb(un_pinned_knights);
		Bitboard knight_attacks = tables::attacks<KNIGHT>(s, data.all);
		if constexpr (move_gen_type == ALL) push<QUIET>(s, knight_attacks & quiet_mask);
		push<CAPTURE_TYPE>(s, knight_attacks & capture_mask);
	}

	Bitboard non_pinned_diag = data.us_diag_sliders & ~pinned;
	while (non_pinned_diag) {
		Square s = pop_lsb(non_pinned_diag);
		Bitboard non_pinned_diag_attacks = tables::attacks<BISHOP>(s, data.all);

		if constexpr (move_gen_type == ALL) push<QUIET>(s, non_pinned_diag_attacks & quiet_mask);
		push<CAPTURE_TYPE>(s, non_pinned_diag_attacks & capture_mask);
	}

	Bitboard non_pinned_ortho = data.us_ortho_sliders & ~pinned;
	while (non_pinned_ortho) {
		Square s = pop_lsb(non_pinned_ortho);
		Bitboard non_pinned_ortho_attacks = tables::attacks<ROOK>(s, data.all);

		if constexpr (move_gen_type == ALL) push<QUIET>(s, non_pinned_ortho_attacks & quiet_mask);
		push<CAPTURE_TYPE>(s, non_pinned_ortho_attacks & capture_mask);
	}
}

template<Color color, MoveGenerationType move_gen_type>
constexpr void MoveList<color, move_gen_type>::push_non_pinned_pawns(const MoveList::SharedData &data, Bitboard pinned,
																	 Bitboard quiet_mask, Bitboard capture_mask) {

	Bitboard non_pinned_pawns = board_.occupancy<color, PAWN>() & ~pinned & ~MASK_RANK[relative_rank<color>(RANK7)];

	Bitboard left_pawn_captures = shift_relative<color, NORTH_WEST>(non_pinned_pawns) & capture_mask;
	Bitboard right_pawn_captures = shift_relative<color, NORTH_EAST>(non_pinned_pawns) & capture_mask;

	while (left_pawn_captures) {
		Square s = pop_lsb(left_pawn_captures);
		push_single<CAPTURE_TYPE>(s - relative_dir<color, NORTH_WEST>(), s);
	}

	while (right_pawn_captures) {
		Square s = pop_lsb(right_pawn_captures);
		push_single<CAPTURE_TYPE>(s - relative_dir<color, NORTH_EAST>(), s);
	}

	if constexpr (move_gen_type == CAPTURES) return;

	Bitboard single_pawn_pushes = shift_relative<color, NORTH>(non_pinned_pawns) & ~data.all;

	Bitboard double_pawn_pushes = single_pawn_pushes & MASK_RANK[relative_rank<color>(RANK3)];
	double_pawn_pushes = shift_relative<color, NORTH>(double_pawn_pushes) & quiet_mask;

	single_pawn_pushes &= quiet_mask;

	while (single_pawn_pushes) {
		Square s = pop_lsb(single_pawn_pushes);
		push_single<QUIET>(s - relative_dir<color, NORTH>(), s);
	}

	while (double_pawn_pushes) {
		Square s = pop_lsb(double_pawn_pushes);
		push_single<DOUBLE_PUSH>(s - relative_dir<color, NORTH_NORTH>(), s);
	}
}

template<Color color, MoveGenerationType move_gen_type>
MoveList<color, move_gen_type>::MoveList(Position &board) : board_{board} {

	const SharedData data(board_);

	Bitboard danger = generate_danger(data);

	push_check_evasions(data, danger);

	Bitboard checkers{}, pinned{};
	std::tie(checkers, pinned) = generate_checkers_and_pinned(data);

	Bitboard capture_mask, quiet_mask;
	switch (pop_count(checkers)) {
		case 2: return;
		case 1:
			if (push_pawn_knight_check_captures(data, checkers, pinned)) return;
			capture_mask = checkers;
			quiet_mask = tables::square_in_between(data.us_king_square, lsb(checkers));
			break;
		default:
			capture_mask = data.them_occupancy;
			quiet_mask = ~data.all;
			push_en_passant(data, pinned);
			push_castle(data, danger);
			push_pinned(data, pinned, quiet_mask, capture_mask);
			break;
	}
	push_non_pinned_pieces(data, pinned, quiet_mask, capture_mask);
	push_non_pinned_pawns(data, pinned, quiet_mask, capture_mask);
	push_promotions(pinned, quiet_mask, capture_mask);
}
