//
// Created by Alex Tian on 12/2/2022.
//

#include <stdexcept>
#include <iostream>
#include <sstream>
#include "position.h"
#include "types/bitboard.h"
#include "constants/misc_constants.h"
#include "constants/zobrist_constants.h"
#include "../utils/helpers.h"

Position::Position(const std::string& fen) {
	set_fen(fen);
}

void Position::reset() {
	state_history.clear();

	pieces.fill(0);
	board.fill(NO_PIECE);

	side = WHITE;
}

template<bool update_hash>
void Position::place_piece(Piece piece, Square square) {
	pieces[piece] |= square_to_bitboard(square);
	board[square] = piece;
	if constexpr (update_hash) {
		state_history.top().hash ^= ZOBRIST_PIECE_SQUARE[piece][square];
	}
}

template<bool update_hash>
void Position::remove_piece(Square square) {
	if constexpr (update_hash) {
		state_history.top().hash ^= ZOBRIST_PIECE_SQUARE[piece_at(square)][square];
	}
	pieces[piece_at(square)] &= ~square_to_bitboard(square);
	board[square] = NO_PIECE;
}

template<bool update_hash>
void Position::move_piece(Square from, Square to) {
	Piece piece = piece_at(from);
	remove_piece<update_hash>(from);
	place_piece<update_hash>(piece, to);
}

u8 Position::castling_state(Bitboard from_to) const {
	i32 white_oo = !(from_to & PositionState::WHITE_OO_BANNED_MASK) << 3;
	i32 white_ooo = !(from_to & PositionState::WHITE_OOO_BANNED_MASK) << 2;
	i32 black_oo = !(from_to & PositionState::BLACK_OO_BANNED_MASK) << 1;
	i32 black_ooo = !(from_to & PositionState::BLACK_OOO_BANNED_MASK);
	return white_oo | white_ooo | black_oo | black_ooo;
}

std::string Position::fen() const {
	std::ostringstream fen;
	i32 empty;

	for (i32 i = 56; i >= 0; i -= 8) {
		empty = 0;
		for (i32 j = 0; j < 8; j++) {
			Piece p = board[i + j];
			if (p == NO_PIECE) empty++;
			else {
				fen << (empty == 0 ? "" : std::to_string(empty)) << PIECE_MATCHER[p];
				empty = 0;
			}
		}

		if (empty != 0) fen << empty;
		if (i > 0) fen << '/';
	}

	std::string castling_rights;
	const Bitboard set_castling_state = castling_state(state_history.peek().from_to);
	if ((set_castling_state >> 3) & 0b1) castling_rights += "K";
	if ((set_castling_state >> 2) & 0b1) castling_rights += "Q";
	if ((set_castling_state >> 1) & 0b1) castling_rights += "k";
	if (set_castling_state & 0b1) castling_rights += "q";
	if (set_castling_state == 0) castling_rights = "-";

	fen << (side == WHITE ? " w " : " b ")
		<< castling_rights
		<< (ep_square() == NO_SQUARE ? " -" : " " + std::string(SQ_TO_STRING[ep_square()]))
		<< " "
		<< "0" << " "
		<< "1";
	return fen.str();
}

void Position::set_fen(const std::string& fen_string) {
	reset();

	// Push empty state to state history.
	state_history.push({});

	std::vector<std::string> fen_tokens = split(fen_string, " ");

	if (fen_tokens.size() < 4) {
		throw std::invalid_argument("Fen is missing fields. ");
	}

	const std::string position = fen_tokens[0];
	const std::string player = fen_tokens[1];
	const std::string castling = fen_tokens[2];
	const std::string en_passant = fen_tokens[3];

	const std::string half_move_clock = fen_tokens.size() > 4 ? fen_tokens[4] : "0";
	const std::string full_move_counter = fen_tokens.size() > 4 ? fen_tokens[5] : "1";

	side = player == "w" ? WHITE : BLACK;
	state_history.top().hash ^= ZOBRIST_COLOR[side];

	Square square = a8;

	for (char ch : position) {
		if (isdigit(ch)) square += std::stoi(std::string(1, ch)) * EAST;
		else if (ch == '/') square += SOUTH_SOUTH;
		else place_piece<ENABLE_HASH_UPDATE>(piece_from_char(ch), square++);
	}

	state_history.top().from_to = PositionState::NO_CASTLING_MASK;
	for (char c : castling) {
		if (c == 'K') 		state_history.top().from_to &= ~PositionState::WHITE_OO_BANNED_MASK;
		else if (c == 'Q') 	state_history.top().from_to &= ~PositionState::WHITE_OOO_BANNED_MASK;
		else if (c == 'k') 	state_history.top().from_to &= ~PositionState::BLACK_OO_BANNED_MASK;
		else if (c == 'q') 	state_history.top().from_to &= ~PositionState::BLACK_OOO_BANNED_MASK;
	}
	state_history.top().hash ^= ZOBRIST_CASTLING_RIGHTS[castling_state(state_history.top().from_to)];

	if (en_passant.size() > 1) {
		auto s = create_square(File(en_passant[0] - 'a'), Rank(en_passant[1] - '1'));
		state_history.top().ep_square = s;
	} else {
		state_history.top().ep_square = NO_SQUARE;
	}
	state_history.top().hash ^= ZOBRIST_EP_SQUARE[state_history.top().ep_square];
}

std::ostream& operator << (std::ostream& os, const Position& p) {
	const std::string s = "   +---+---+---+---+---+---+---+---+\n";
	const std::string t = "     A   B   C   D   E   F   G   H\n";
	os << t;
	for (i32 i = 56; i >= 0; i -= 8) {
		os << s << " " << i / 8 + 1 << " ";
		for (i32 j = 0; j < 8; j++)
			os << "| " << PIECE_MATCHER[p.board[i + j]] << " ";
		os << "| " << i / 8 + 1 << "\n";
	}
	os << s;
	os << t << "\n";

	os << "FEN: " << p.fen() << "\n";
	os << "Hash: 0x" << std::hex << p.hash() << std::dec << "\n";

	return os;
}

bool Position::has_repetition(Repetition fold) {
	int count = fold == THREE_FOLD ? 0 : 1;
	const i32 hash_hist_size = static_cast<int>(state_history.size());
	const u64 current_hash = hash();
	for (i32 idx = hash_hist_size - 3;
		 idx >= 0 && idx >= hash_hist_size - fifty_move_rule();
		 idx -= 2) {
		ZobristHash stack_hash = state_history[idx].hash;
		if (stack_hash == current_hash) count += 1;
		if (count >= 2) return true;
	}
	return false;
}

template<Color color>
void Position::play(Move move) {
	PositionState next_state = {};
	next_state.from_to = state_history.peek().from_to | square_to_bitboard(move.from()) | square_to_bitboard(move.to());
	next_state.captured = piece_at(move.to());
	next_state.hash = state_history.peek().hash;
	next_state.fifty_move_rule = state_history.peek().fifty_move_rule + 1;
	next_state.ep_square = NO_SQUARE;

	if (move.is_capture() || type_of(piece_at(move.from())) == PAWN)
		next_state.fifty_move_rule = 0;

	next_state.hash ^= ZOBRIST_COLOR[~color];
	next_state.hash ^= ZOBRIST_COLOR[color];
	next_state.hash ^= ZOBRIST_CASTLING_RIGHTS[castling_state(state_history.peek().from_to)];
	next_state.hash ^= ZOBRIST_CASTLING_RIGHTS[castling_state(next_state.from_to)];
	next_state.hash ^= ZOBRIST_EP_SQUARE[state_history.peek().ep_square];
	state_history.push(next_state);

	if (move.type() & CAPTURE_TYPE && move.type() != ENPASSANT) remove_piece<ENABLE_HASH_UPDATE>(move.to());
	move_piece<ENABLE_HASH_UPDATE>(move.from(), move.to());

	MoveType type = move.type();
	switch (type) {
		case DOUBLE_PUSH:
			state_history.top().ep_square = move.from() + relative_dir<color, NORTH>();
			break;
		case OO:
			if constexpr (color == WHITE) move_piece<ENABLE_HASH_UPDATE>(h1, f1);
			else move_piece<ENABLE_HASH_UPDATE>(h8, f8);
			break;
		case OOO:
			if constexpr (color == WHITE) move_piece<ENABLE_HASH_UPDATE>(a1, d1);
			else move_piece<ENABLE_HASH_UPDATE>(a8, d8);
			break;
		case ENPASSANT:
			remove_piece<ENABLE_HASH_UPDATE>(move.to() + relative_dir<color, SOUTH>());
			state_history.top().captured = make_piece<~color, PAWN>();
			break;
		case PR_KNIGHT | CAPTURE_TYPE:
		case PR_KNIGHT:
			remove_piece<ENABLE_HASH_UPDATE>(move.to());
			place_piece<ENABLE_HASH_UPDATE>(make_piece<color, KNIGHT>(), move.to());
			break;
		case PR_BISHOP | CAPTURE_TYPE:
		case PR_BISHOP:
			remove_piece<ENABLE_HASH_UPDATE>(move.to());
			place_piece<ENABLE_HASH_UPDATE>(make_piece<color, BISHOP>(), move.to());
			break;
		case PR_ROOK | CAPTURE_TYPE:
		case PR_ROOK:
			remove_piece<ENABLE_HASH_UPDATE>(move.to());
			place_piece<ENABLE_HASH_UPDATE>(make_piece<color, ROOK>(), move.to());
			break;
		case PR_QUEEN | CAPTURE_TYPE:
		case PR_QUEEN:
			remove_piece<ENABLE_HASH_UPDATE>(move.to());
			place_piece<ENABLE_HASH_UPDATE>(make_piece<color, QUEEN>(), move.to());
			break;
		default: break;
	}
	state_history.top().hash ^= ZOBRIST_EP_SQUARE[state_history.peek().ep_square];
	side = ~side;
}

template<Color color>
void Position::undo(Move move) {
	PositionState old_state = state_history.pop();

	move_piece<DISABLE_HASH_UPDATE>(move.to(), move.from());
	place_piece<DISABLE_HASH_UPDATE>(old_state.captured, move.to());

	MoveType type = move.type();
	switch (type) {
		case OO:
			if constexpr (color == WHITE) move_piece<DISABLE_HASH_UPDATE>(f1, h1);
			else move_piece<DISABLE_HASH_UPDATE>(f8, h8);
			break;
		case OOO:
			if constexpr (color == WHITE) move_piece<DISABLE_HASH_UPDATE>(d1, a1);
			else move_piece<DISABLE_HASH_UPDATE>(d8, a8);
			break;
		case ENPASSANT:
			remove_piece<DISABLE_HASH_UPDATE>(move.to());
			place_piece<DISABLE_HASH_UPDATE>(make_piece<~color, PAWN>(), move.to() + relative_dir<color, SOUTH>());
			break;
		case PR_KNIGHT | CAPTURE_TYPE:
		case PR_KNIGHT:
		case PR_BISHOP | CAPTURE_TYPE:
		case PR_BISHOP:
		case PR_ROOK | CAPTURE_TYPE:
		case PR_ROOK:
		case PR_QUEEN | CAPTURE_TYPE:
		case PR_QUEEN:
			remove_piece<DISABLE_HASH_UPDATE>(move.from());
			place_piece<DISABLE_HASH_UPDATE>(make_piece<color, PAWN>(), move.from());
			break;
		default: break;
	}
	side = ~side;
}

template<Color color>
void Position::play_null() {
	PositionState next_state = {};
	next_state.from_to = state_history.peek().from_to;
	next_state.captured = NO_PIECE;
	next_state.hash = state_history.peek().hash;
	next_state.fifty_move_rule = state_history.peek().fifty_move_rule + 1;
	next_state.ep_square = NO_SQUARE;

	next_state.hash ^= ZOBRIST_COLOR[~color];
	next_state.hash ^= ZOBRIST_COLOR[color];

	state_history.push(next_state);
}

template<Color color>
void Position::undo_null() {
	state_history.pop();
}

template void Position::place_piece<Position::ENABLE_HASH_UPDATE>(Piece piece, Square square);
template void Position::place_piece<Position::DISABLE_HASH_UPDATE>(Piece piece, Square square);

template void Position::remove_piece<Position::ENABLE_HASH_UPDATE>(Square square);
template void Position::remove_piece<Position::DISABLE_HASH_UPDATE>(Square square);

template void Position::move_piece<Position::ENABLE_HASH_UPDATE>(Square from, Square to);
template void Position::move_piece<Position::DISABLE_HASH_UPDATE>(Square from, Square to);

template void Position::play<WHITE>(Move move);
template void Position::play<BLACK>(Move move);

template void Position::undo<WHITE>(Move move);
template void Position::undo<BLACK>(Move move);

template void Position::play_null<WHITE>();
template void Position::play_null<BLACK>();

template void Position::undo_null<WHITE>();
template void Position::undo_null<BLACK>();
