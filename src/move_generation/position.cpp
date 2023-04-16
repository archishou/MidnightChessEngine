#include "position.h"
#include "tables.h"
#include "utils/helpers.h"
#include <sstream>

//Zobrist keys for each piece and each square
//Used to incrementally update the hash key of a position
uint64_t zobrist::zobrist_piece_table[NPIECES][NSQUARES];
uint64_t zobrist::zobrist_ep_file_table[NFILES + 1];
uint64_t zobrist::zobrist_castling_rights_table[NCASTLING_RIGHTS];
uint64_t zobrist::zobrist_color_key;

void zobrist::initialise_zobrist_keys() {
	PRNG rng(70026072);
	for (auto & i : zobrist::zobrist_piece_table)
		for (int j = 0; j < NSQUARES; j++)
			i[j] = rng.rand<uint64_t>();
	for (unsigned long long & i : zobrist::zobrist_castling_rights_table) {
		i = rng.rand<uint64_t>();
	}
	for (unsigned long long & i : zobrist::zobrist_ep_file_table) {
		i = rng.rand<uint64_t>();
	}
	zobrist_color_key = rng.rand<uint64_t>();
}

std::ostream& operator<< (std::ostream& os, const Position& p) {
	const char* s = "   +---+---+---+---+---+---+---+---+\n";
	const char* t = "     A   B   C   D   E   F   G   H\n";
	os << t;
	for (int i = 56; i >= 0; i -= 8) {
		os << s << " " << i / 8 + 1 << " ";
		for (int j = 0; j < 8; j++)
			os << "| " << PIECE_STR[p.board[i + j]] << " ";
		os << "| " << i / 8 + 1 << "\n";
	}
	os << s;
	os << t << "\n";

	os << "FEN: " << p.fen() << "\n";
	os << "Hash: 0x" << std::hex << p.hash << std::dec << "\n";

	return os;
}

std::string Position::fen() const {
	std::ostringstream fen;
	int empty;

	for (int i = 56; i >= 0; i -= 8) {
		empty = 0;
		for (int j = 0; j < 8; j++) {
			Piece p = board[i + j];
			if (p == NO_PIECE) empty++;
			else {
				fen << (empty == 0 ? "" : std::to_string(empty))
					<< PIECE_STR[p];
				empty = 0;
			}
		}

		if (empty != 0) fen << empty;
		if (i > 0) fen << '/';
	}

	std::string castling_rights;
	const Bitboard set_castling_state = castling_state();
	if ((set_castling_state >> 3) & 0b1) castling_rights += "K";
	if ((set_castling_state >> 2) & 0b1) castling_rights += "Q";
	if ((set_castling_state >> 1) & 0b1) castling_rights += "k";
	if (set_castling_state & 0b1) castling_rights += "q";
	if (set_castling_state == 0) castling_rights = "-";
	int full_move_clock = half_move_clock / 2;
	if (side_to_play == WHITE) full_move_clock += 1;

	fen << (side_to_play == WHITE ? " w " : " b ")
		<< castling_rights
		<< (history[game_ply].epsq == NO_SQUARE ? " -" : " " + std::string(SQSTR[history[game_ply].epsq]))
		<< " "
		<< fifty_mr_clock() << " "
		<< full_move_clock;

	return fen.str();
}

void Position::set(const std::string& fen, Position& p) {
    p.clear();

	std::vector<std::string> fen_components = split(fen, " ");
	std::string position = fen_components[0];
	std::string side_to_play = fen_components[1];
	std::string castling_rights = fen_components[2];
	std::string ep_square = fen_components[3];

	std::string half_move_clock = "0";
	std::string full_move_clock = "1";

	if (fen_components.size() > 5) {
		half_move_clock = fen_components[4];
		full_move_clock = fen_components[5];
	}

	int square = a8;
	for (char ch : position) {
		if (isdigit(ch)) square += (ch - '0') * EAST;
		else if (ch == '/') square += 2 * SOUTH;
		else p.put_piece(Piece(PIECE_STR.find(ch)), Square(square++));
	}

	p.side_to_play = side_to_play == "w" ? WHITE : BLACK;
	p.history[p.game_ply].entry = ALL_CASTLING_MASK;

	for (char token : castling_rights) {
		switch (token) {
			case 'K':
				p.history[p.game_ply].entry &= ~WHITE_OO_MASK;
				break;
			case 'Q':
				p.history[p.game_ply].entry &= ~WHITE_OOO_MASK;
				break;
			case 'k':
				p.history[p.game_ply].entry &= ~BLACK_OO_MASK;
				break;
			case 'q':
				p.history[p.game_ply].entry &= ~BLACK_OOO_MASK;
				break;
			default:
				continue;
		}
	}

	if (ep_square != "-") {
		p.history[p.game_ply].epsq = create_square(File(ep_square[0] - 'a'), Rank(ep_square[1] - '1'));
	} else {
		p.history[p.game_ply].epsq = NO_SQUARE;
	}

	p.history[p.game_ply].fifty_mr_clock = std::stoi(half_move_clock);
	p.half_move_clock = std::stoi(full_move_clock);

	if (p.side_to_play == BLACK) p.hash ^= zobrist::zobrist_color_key;
	p.hash ^= zobrist::zobrist_castling_rights_table[p.castling_state()];
	p.hash ^= zobrist::zobrist_ep_file_table[p.ep_file()];
}

void Position::move_piece(Square from, Square to) {
	hash ^= zobrist::zobrist_piece_table[board[from]][from] ^ zobrist::zobrist_piece_table[board[from]][to]
			^ zobrist::zobrist_piece_table[board[to]][to];
	Bitboard mask = SQUARE_BB[from] | SQUARE_BB[to];
	piece_bb[board[from]] ^= mask;
	piece_bb[board[to]] &= ~mask;
	board[to] = board[from];
	board[from] = NO_PIECE;
}

void Position::move_piece_quiet(Square from, Square to) {
	hash ^= zobrist::zobrist_piece_table[board[from]][from] ^ zobrist::zobrist_piece_table[board[from]][to];
	piece_bb[board[from]] ^= (SQUARE_BB[from] | SQUARE_BB[to]);
	board[to] = board[from];
	board[from] = NO_PIECE;
}

void Position::update_hash_board_features(Bitboard original_castle_state, int original_ep_file) {
	hash ^= zobrist::zobrist_color_key;
	Bitboard new_castling_state = castling_state();
	if (new_castling_state != original_castle_state) {
		hash ^= zobrist::zobrist_castling_rights_table[original_castle_state];
		hash ^= zobrist::zobrist_castling_rights_table[new_castling_state];
	}
	int new_ep_file = ep_file();
	if (original_ep_file != new_ep_file) {
		hash ^= zobrist::zobrist_ep_file_table[original_ep_file];
		hash ^= zobrist::zobrist_ep_file_table[new_ep_file];
	}
}

void Position::clear() {
    side_to_play = WHITE;
    game_ply = 0;
    hash = 0;
    pinned = 0;
    checkers = 0;

    for (unsigned long long & i : piece_bb) { i = 0; }
    for (auto & i : board) { i = NO_PIECE; }
	for (int i = 0; i < 300; i++) history[i] = UndoInfo();
	hash_history.clear();
}
