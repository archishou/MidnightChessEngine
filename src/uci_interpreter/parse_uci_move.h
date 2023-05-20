//
// Created by Archishmaan Peyyety on 2/1/23.
//
#pragma once
#include "../board/position.h"
#include "../utils/helpers.h"
#include <locale>

inline char promotion_character(std::string uci_move) {
	char promotion_piece = uci_move.at(4);
	char lower_case = std::tolower(promotion_piece, std::locale());
	return lower_case;
}

inline Move uci_to_move(const std::string& moveStr, Position& position) {
	Move move = Move(moveStr.substr(0, 4));
	// Pawn Promotion
	if (moveStr.size() == 5) {
		// Quiet Promotion
		char p_char = promotion_character(moveStr);
		if (position.piece_at(move.to()) == NO_PIECE) {
			if (p_char == 'q') return Move(move.from(), move.to(), PR_QUEEN);
			if (p_char == 'b') return Move(move.from(), move.to(), PR_BISHOP);
			if (p_char == 'n') return Move(move.from(), move.to(), PR_KNIGHT);
			if (p_char == 'r') return Move(move.from(), move.to(), PR_ROOK);
		}
		if (p_char == 'q') return Move(move.from(), move.to(), PR_QUEEN | CAPTURE_TYPE);
		if (p_char == 'b') return Move(move.from(), move.to(), PR_BISHOP | CAPTURE_TYPE);
		if (p_char == 'n') return Move(move.from(), move.to(), PR_KNIGHT | CAPTURE_TYPE);
		if (p_char == 'r') return Move(move.from(), move.to(), PR_ROOK | CAPTURE_TYPE);
	}

	// En Passant
	if (position.piece_at(move.to()) == NO_PIECE && type_of(position.piece_at(move.from())) == PAWN &&
		file_of(move.to()) != file_of(move.from())) {
		return Move(move.from(), move.to(), ENPASSANT);
	}

	if (type_of(position.piece_at(move.from())) == PAWN && abs(rank_of(move.to()) - rank_of(move.from())) == 2) {
		return Move(move.from(), move.to(), DOUBLE_PUSH);
	}

	// Castle
	if (type_of(position.piece_at(move.from())) == KING) {
		if (moveStr == "e1g1" || moveStr == "e8g8") return Move(move.from(), move.to(), OO);
		if (moveStr == "e1c1" || moveStr == "e8c8") return Move(move.from(), move.to(), OOO);
	}

	// Capture
	if (position.piece_at(move.to()) != NO_PIECE) {
		return Move(move.from(), move.to(), CAPTURE_TYPE);
	}

	return {move.from(), move.to(), QUIET};
}

inline void uci_update_position_from_moves(Position& board, const std::string& uci_move_string) {
	std::vector<std::string> uci_moves = split(uci_move_string, " ");
	for (const std::string& uci_move : uci_moves) {
		if (uci_move.empty()) return;
		Move nextMove = uci_to_move(uci_move, board);
		if (board.turn() == BLACK) board.play<BLACK>(nextMove);
		else board.play<WHITE>(nextMove);
	}
}
