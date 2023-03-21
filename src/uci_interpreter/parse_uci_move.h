//
// Created by Archishmaan Peyyety on 2/1/23.
//
#pragma once
#include "move_generation/position.h"
#include "helpers.h"

char promotion_character(std::string uci_move) {
	char promotion_piece = uci_move.at(4);
	char lower_case = std::tolower(promotion_piece, std::locale());
	return lower_case;
}

Move uci_to_move(const std::string& moveStr, Position& position) {
	Move move = Move(moveStr.substr(0, 4));
	// Pawn Promotion
	if (moveStr.size() == 5) {
		// Quiet Promotion
		char p_char = promotion_character(moveStr);
		if (position.at(move.to()) == NO_PIECE) {
			if (p_char == 'q') return Move(move.from(), move.to(), PR_QUEEN);
			if (p_char == 'b') return Move(move.from(), move.to(), PR_BISHOP);
			if (p_char == 'n') return Move(move.from(), move.to(), PR_KNIGHT);
			if (p_char == 'r') return Move(move.from(), move.to(), PR_ROOK);
		}
		if (p_char == 'q') return Move(move.from(), move.to(), PC_QUEEN);
		if (p_char == 'b') return Move(move.from(), move.to(), PC_BISHOP);
		if (p_char == 'n') return Move(move.from(), move.to(), PC_KNIGHT);
		if (p_char == 'r') return Move(move.from(), move.to(), PC_ROOK);
	}

	// En Passant
	if (position.at(move.to()) == NO_PIECE && type_of(position.at(move.from())) == PAWN &&
		file_of(move.to()) != file_of(move.from())) {
		return Move(move.from(), move.to(), EN_PASSANT);
	}

	if (type_of(position.at(move.from())) == PAWN && abs(rank_of(move.to()) - rank_of(move.from())) == 2) {
		return Move(move.from(), move.to(), DOUBLE_PUSH);
	}

	// Castle
	if (type_of(position.at(move.from())) == KING) {
		if (moveStr == "e1g1" || moveStr == "e8g8") return Move(move.from(), move.to(), OO);
		if (moveStr == "e1c1" || moveStr == "e8c8") return Move(move.from(), move.to(), OOO);
	}

	// Capture
	if (position.at(move.to()) != NO_PIECE) {
		return Move(move.from(), move.to(), CAPTURE);
	}

	return {move.from(), move.to(), QUIET};
}

void uci_update_position_from_moves(Position& board, const std::string& uci_move_string) {
	std::vector<std::string> uci_moves = split(uci_move_string, " ");
	for (const std::string& uci_move : uci_moves) {
		if (uci_move.empty()) return;
		Move nextMove = uci_to_move(uci_move, board);
		if (board.turn() == BLACK) board.play<BLACK>(nextMove);
		else board.play<WHITE>(nextMove);
	}
}