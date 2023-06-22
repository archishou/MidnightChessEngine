//
// Created by Archishmaan Peyyety on 6/21/23.
//
#include "uci_move_parse.h"

char promotion_character(string uci_move) {
	auto promotion_piece = uci_move.at(4);
	auto lower_case = std::tolower(promotion_piece, std::locale());
	return lower_case;
}

Move uci_to_move(const string& moveStr, Position& position) {
	Move move = Move(moveStr.substr(0, 4));
	// Pawn Promotion
	if (moveStr.size() == 5) {
		// Quiet Promotion
		auto p_char = promotion_character(moveStr);
		if (position.piece_at(move.to()) == NO_PIECE) {
			if (p_char == 'q') return {move.from(), move.to(), PR_QUEEN};
			if (p_char == 'b') return {move.from(), move.to(), PR_BISHOP};
			if (p_char == 'n') return {move.from(), move.to(), PR_KNIGHT};
			if (p_char == 'r') return {move.from(), move.to(), PR_ROOK};
		}
		if (p_char == 'q') return {move.from(), move.to(), PR_QUEEN | CAPTURE_TYPE};
		if (p_char == 'b') return {move.from(), move.to(), PR_BISHOP | CAPTURE_TYPE};
		if (p_char == 'n') return {move.from(), move.to(), PR_KNIGHT | CAPTURE_TYPE};
		if (p_char == 'r') return {move.from(), move.to(), PR_ROOK | CAPTURE_TYPE};
	}

	// En Passant
	if (position.piece_at(move.to()) == NO_PIECE && type_of(position.piece_at(move.from())) == PAWN &&
		file_of(move.to()) != file_of(move.from())) {
		return {move.from(), move.to(), ENPASSANT};
	}

	if (type_of(position.piece_at(move.from())) == PAWN && abs(rank_of(move.to()) - rank_of(move.from())) == 2) {
		return {move.from(), move.to(), DOUBLE_PUSH};
	}

	// Castle
	if (type_of(position.piece_at(move.from())) == KING) {
		if (moveStr == "e1g1" || moveStr == "e8g8") return {move.from(), move.to(), OO};
		if (moveStr == "e1c1" || moveStr == "e8c8") return {move.from(), move.to(), OOO};
	}

	// Capture
	if (position.piece_at(move.to()) != NO_PIECE) {
		return {move.from(), move.to(), CAPTURE_TYPE};
	}

	return {move.from(), move.to(), QUIET};
}

void uci_update_position_from_moves(Position& board, const string& uci_move_string) {
	std::vector<string> uci_moves = split(uci_move_string, " ");
	for (const string& uci_move : uci_moves) {
		if (uci_move.empty()) return;
		Move nextMove = uci_to_move(uci_move, board);
		if (board.turn() == BLACK) board.play<BLACK>(nextMove);
		else board.play<WHITE>(nextMove);
	}
}
