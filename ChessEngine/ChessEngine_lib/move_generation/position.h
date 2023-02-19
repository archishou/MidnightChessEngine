#pragma once

#include "types.h"
#include <ostream>
#include <string>
#include "tables.h"
#include <utility>

//A psuedorandom number generator
//Source: Stockfish
class PRNG {
	uint64_t s;

	uint64_t rand64() {
		s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
		return s * 2685821657736338717LL;
	}

public:
	PRNG(uint64_t seed) : s(seed) {}

	//Generate psuedorandom number
	template<typename T> T rand() { return T(rand64()); }

	//Generate psuedorandom number with only a few set bits
	template<typename T> 
	T sparse_rand() {
		return T(rand64() & rand64() & rand64());
	}
};


namespace zobrist {
	extern uint64_t zobrist_piece_table[NPIECES][NSQUARES];
	extern uint64_t zobrist_castling_rights_table[NCASTLING_RIGHTS];
	extern uint64_t zobrist_ep_file_table[NFILES + 1];
	extern uint64_t zobrist_color_key;
	extern void initialise_zobrist_keys();
}

struct MoveGenerationOptions {
	bool generate_captures;
	bool generate_checks;
	bool generate_promotion;
	bool generate_quiet;
};

//Stores position information which cannot be recovered on undo-ing a move
struct UndoInfo {
	//The bitboard of squares on which pieces have either moved from, or have been moved to. Used for castling
	//legality checks
	Bitboard entry;

	//The piece that was captured on the last move
	Piece captured;
	
	//The en passant square. This is the square which pawns can move to in order to en passant capture an enemy pawn that has 
	//double pushed on the previous move
	Square epsq;

	short fifty_mr_clock;

	constexpr UndoInfo() : entry(0), captured(NO_PIECE), epsq(NO_SQUARE), fifty_mr_clock(0) {}
	
	//This preserves the entry bitboard across moves
	UndoInfo(const UndoInfo& prev) :
			entry(prev.entry), captured(NO_PIECE), epsq(NO_SQUARE), fifty_mr_clock(prev.fifty_mr_clock) {}
};

class Position {
private:
	//A bitboard of the locations of each piece
	Bitboard piece_bb[NPIECES];
	
	//A mailbox representation of the board. Stores the piece occupying each square on the board
	Piece board[NSQUARES];
	
	//The side whose turn it is to play next
	Color side_to_play;
	
	//The current game ply (depth), incremented after each move 
	int game_ply;

	//The zobrist hash of the position, which can be incrementally updated and rolled back after each
	//make/unmake
	zobrist_hash hash;

	template<Color Us>
	Move *generate_captures(Move *list);

public:
	//The history of non-recoverable information
    //Longest game in history was
    //269 ply or 538 moves between Ivan Nikolic vs. Goran Arsovic, Belgrade, 1989.
    //The game ended in a draw. 500 should be plenty!
	UndoInfo history[269 * 2 + 50];
	
	//The bitboard of enemy pieces that are currently attacking the king, updated whenever generate_moves()
	//is called
	Bitboard checkers;
	
	//The bitboard of pieces that are currently pinned to the king by enemy sliders, updated whenever 
	//generate_moves() is called
	Bitboard pinned;

	std::vector<zobrist_hash> hash_history;

	Position() : piece_bb{ 0 }, side_to_play(WHITE), game_ply(0), half_move_clock(1), board{},
				 hash(0), pinned(0), checkers(0) {

		//Sets all squares on the board as empty
		for (int i = 0; i < 64; i++) board[i] = NO_PIECE;
		history[0] = UndoInfo();
	}

    void clear();
    static bool equality(Position& p1, Position&p2);

    static void set(const std::string& fen, Position& p);
	//Places a piece on a particular square and updates the hash. Placing a piece on a square that is
	//already occupied is an error
	inline void put_piece(Piece pc, Square s) {
		board[s] = pc;
		piece_bb[pc] |= SQUARE_BB[s];
		hash ^= zobrist::zobrist_piece_table[pc][s];
	}

	//Removes a piece from a particular square and updates the hash. 
	inline void remove_piece(Square s) {
		hash ^= zobrist::zobrist_piece_table[board[s]][s];
		piece_bb[board[s]] &= ~SQUARE_BB[s];
		board[s] = NO_PIECE;
	}

	void move_piece(Square from, Square to);
	void move_piece_quiet(Square from, Square to);

	friend std::ostream& operator<<(std::ostream& os, const Position& p);
	std::string fen() const;

	Position& operator=(const Position&) = delete;
	inline bool operator==(const Position& other) const { return hash == other.hash; }

	const Bitboard castling_state() const {
		Bitboard entry = history[game_ply].entry;
		int white_oo = !((bool) (entry & WHITE_OO_MASK)) << 3;
		int white_ooo = !((bool) (entry & WHITE_OOO_MASK)) << 2;
		int black_oo = !((bool) (entry & BLACK_OO_MASK)) << 1;
		int black_ooo = !((bool) (entry & BLACK_OOO_MASK));
		return white_ooo | black_oo | white_oo | black_ooo;
	}

	inline Bitboard bitboard_of(Piece pc) const { return piece_bb[pc]; }
	inline Bitboard bitboard_of(Color c, PieceType pt) const { return piece_bb[make_piece(c, pt)]; }
	inline Piece at(Square sq) const { return board[sq]; }
	inline Color turn() const { return side_to_play; }
	inline int ply() const { return game_ply; }

	inline Square ep_square() const { return history[game_ply].epsq; }
	short ep_file() const { return ep_square() == NO_SQUARE ? NFILES : file_of(ep_square()); }

	void update_hash_board_features(Bitboard original_castle_state, int original_ep_file);
	inline zobrist_hash get_hash() const { return hash; }

	template<Color C> inline Bitboard diagonal_sliders() const;
	template<Color C> inline Bitboard orthogonal_sliders() const;
	template<Color C> inline Bitboard all_pieces() const;
	template<Color C> inline Bitboard attackers_from(Square s, Bitboard occ) const;

	template<Color C> inline bool in_check() const {
		return attackers_from<~C>(bsf(bitboard_of(C, KING)), all_pieces<WHITE>() | all_pieces<BLACK>());
	}

	template<Color C> void play(Move m);
	template<Color c> void play_null();
	template<Color C> void undo(Move m);
	template<Color c> void undo_null();

	template<Color Us>
	Move *generate_legals(Move *list, const MoveGenerationOptions &options);

	int fifty_mr_clock() const { return history[game_ply].fifty_mr_clock; }
	int half_move_clock;
};

//Returns the bitboard of all bishops and queens of a given color
template<Color C> 
inline Bitboard Position::diagonal_sliders() const {
	return C == WHITE ? piece_bb[WHITE_BISHOP] | piece_bb[WHITE_QUEEN] :
		piece_bb[BLACK_BISHOP] | piece_bb[BLACK_QUEEN];
}

//Returns the bitboard of all rooks and queens of a given color
template<Color C> 
inline Bitboard Position::orthogonal_sliders() const {
	return C == WHITE ? piece_bb[WHITE_ROOK] | piece_bb[WHITE_QUEEN] :
		piece_bb[BLACK_ROOK] | piece_bb[BLACK_QUEEN];
}

//Returns a bitboard containing all the pieces of a given color
template<Color C> 
inline Bitboard Position::all_pieces() const {
	return C == WHITE ? piece_bb[WHITE_PAWN] | piece_bb[WHITE_KNIGHT] | piece_bb[WHITE_BISHOP] |
		piece_bb[WHITE_ROOK] | piece_bb[WHITE_QUEEN] | piece_bb[WHITE_KING] :

		piece_bb[BLACK_PAWN] | piece_bb[BLACK_KNIGHT] | piece_bb[BLACK_BISHOP] |
		piece_bb[BLACK_ROOK] | piece_bb[BLACK_QUEEN] | piece_bb[BLACK_KING];
}

//Returns a bitboard containing all pieces of a given color attacking a particluar square
template<Color C> 
inline Bitboard Position::attackers_from(Square s, Bitboard occ) const {
	return C == WHITE ? (pawn_attacks<BLACK>(s) & piece_bb[WHITE_PAWN]) |
		(attacks<KNIGHT>(s, occ) & piece_bb[WHITE_KNIGHT]) |
		(attacks<BISHOP>(s, occ) & (piece_bb[WHITE_BISHOP] | piece_bb[WHITE_QUEEN])) |
		(attacks<ROOK>(s, occ) & (piece_bb[WHITE_ROOK] | piece_bb[WHITE_QUEEN])) :

		(pawn_attacks<WHITE>(s) & piece_bb[BLACK_PAWN]) |
		(attacks<KNIGHT>(s, occ) & piece_bb[BLACK_KNIGHT]) |
		(attacks<BISHOP>(s, occ) & (piece_bb[BLACK_BISHOP] | piece_bb[BLACK_QUEEN])) |
		(attacks<ROOK>(s, occ) & (piece_bb[BLACK_ROOK] | piece_bb[BLACK_QUEEN]));
}

//Plays a move in the position
template<Color C>
void Position::play(const Move m) {
	Bitboard original_castle_state = castling_state();
	int original_ep_file = ep_file();
	++game_ply;
	half_move_clock += 1;
	history[game_ply] = UndoInfo(history[game_ply - 1]);

	MoveFlag type = m.flag();
	history[game_ply].fifty_mr_clock += 1;
	if (type == CAPTURE || type_of(at(m.from())) == PAWN) {
		history[game_ply].fifty_mr_clock = 0;
	}
	side_to_play = ~side_to_play;
	history[game_ply].entry |= SQUARE_BB[m.to()] | SQUARE_BB[m.from()];

	switch (type) {
	case QUIET:
		//The to square is guaranteed to be empty here
		move_piece_quiet(m.from(), m.to());
		break;
	case DOUBLE_PUSH:
		//The to square is guaranteed to be empty here
		move_piece_quiet(m.from(), m.to());
			
		//This is the square behind the pawn that was double-pushed
		history[game_ply].epsq = m.from() + relative_dir<C>(NORTH);
		break;
	case OO:
		if (C == WHITE) {
			move_piece_quiet(e1, g1);
			move_piece_quiet(h1, f1);
		} else {
			move_piece_quiet(e8, g8);
			move_piece_quiet(h8, f8);
		}			
		break;
	case OOO:
		if (C == WHITE) {
			move_piece_quiet(e1, c1); 
			move_piece_quiet(a1, d1);
		} else {
			move_piece_quiet(e8, c8);
			move_piece_quiet(a8, d8);
		}
		break;
	case EN_PASSANT:
		move_piece_quiet(m.from(), m.to());
		remove_piece(m.to() + relative_dir<C>(SOUTH));
		break;
	case PR_KNIGHT:
		remove_piece(m.from());
		put_piece(make_piece(C, KNIGHT), m.to());
		break;
	case PR_BISHOP:
		remove_piece(m.from());
		put_piece(make_piece(C, BISHOP), m.to());
		break;
	case PR_ROOK:
		remove_piece(m.from());
		put_piece(make_piece(C, ROOK), m.to());
		break;
	case PR_QUEEN:
		remove_piece(m.from());
		put_piece(make_piece(C, QUEEN), m.to());
		break;
	case PC_KNIGHT:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());
		
		put_piece(make_piece(C, KNIGHT), m.to());
		break;
	case PC_BISHOP:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());

		put_piece(make_piece(C, BISHOP), m.to());
		break;
	case PC_ROOK:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());

		put_piece(make_piece(C, ROOK), m.to());
		break;
	case PC_QUEEN:
		remove_piece(m.from());
		history[game_ply].captured = board[m.to()];
		remove_piece(m.to());

		put_piece(make_piece(C, QUEEN), m.to());
		break;
	case CAPTURE:
		history[game_ply].captured = board[m.to()];
		move_piece(m.from(), m.to());
		
		break;
	}
	update_hash_board_features(original_castle_state, original_ep_file);
	hash_history.push_back(hash);
}

template<Color c>
void Position::play_null() {
	int original_ep_file = ep_file();
	Bitboard original_castle_state = castling_state();
	game_ply += 1;
	side_to_play = ~side_to_play;
	UndoInfo null_move_history = UndoInfo();
	null_move_history.entry = history[game_ply - 1].entry;
	null_move_history.captured = NO_PIECE;
	null_move_history.fifty_mr_clock = history[game_ply - 1].fifty_mr_clock + 1;
	null_move_history.epsq = NO_SQUARE;
	history[game_ply] = null_move_history;
	update_hash_board_features(original_castle_state, original_ep_file);
}

//Undos a move in the current position, rolling it back to the previous position
template<Color C>
void Position::undo(const Move m) {
	MoveFlag type = m.flag();
	Bitboard current_castling_state = castling_state();
	int current_ep_file = ep_file();
	switch (type) {
	case QUIET:
		move_piece_quiet(m.to(), m.from());
		break;
	case DOUBLE_PUSH:
		move_piece_quiet(m.to(), m.from());
		break;
	case OO:
		if (C == WHITE) {
			move_piece_quiet(g1, e1);
			move_piece_quiet(f1, h1);
		} else {
			move_piece_quiet(g8, e8);
			move_piece_quiet(f8, h8);
		}
		break;
	case OOO:
		if (C == WHITE) {
			move_piece_quiet(c1, e1);
			move_piece_quiet(d1, a1);
		} else {
			move_piece_quiet(c8, e8);
			move_piece_quiet(d8, a8);
		}
		break;
	case EN_PASSANT:
		move_piece_quiet(m.to(), m.from());
		put_piece(make_piece(~C, PAWN), m.to() + relative_dir<C>(SOUTH));
		break;
	case PR_KNIGHT:
	case PR_BISHOP:
	case PR_ROOK:
	case PR_QUEEN:
		remove_piece(m.to());
		put_piece(make_piece(C, PAWN), m.from());
		break;
	case PC_KNIGHT:
	case PC_BISHOP:
	case PC_ROOK:
	case PC_QUEEN:
		remove_piece(m.to());
		put_piece(make_piece(C, PAWN), m.from());
		put_piece(history[game_ply].captured, m.to());
		break;
	case CAPTURE:
		move_piece_quiet(m.to(), m.from());
		put_piece(history[game_ply].captured, m.to());
		break;
	}
	hash_history.pop_back();
	half_move_clock -= 1;
	side_to_play = ~side_to_play;
	--game_ply;
	update_hash_board_features(current_castling_state, current_ep_file);
}

template<Color C>
void Position::undo_null() {
	int old_ep_file = ep_file();
	Bitboard old_castling_state = castling_state();
	game_ply -= 1;
	side_to_play = ~side_to_play;
	update_hash_board_features(old_castling_state, old_ep_file);
}

template<Color Us>
Move* Position::generate_captures(Move *list) {
	constexpr Color Them = ~Us;

	const Bitboard us_bb = all_pieces<Us>();
	const Bitboard them_bb = all_pieces<Them>();
	const Bitboard all = us_bb | them_bb;

	const Square our_king = bsf(bitboard_of(Us, KING));
	const Square their_king = bsf(bitboard_of(Them, KING));

	const Bitboard our_diag_sliders = diagonal_sliders<Us>();
	const Bitboard their_diag_sliders = diagonal_sliders<Them>();
	const Bitboard our_orth_sliders = orthogonal_sliders<Us>();
	const Bitboard their_orth_sliders = orthogonal_sliders<Them>();

	//General purpose bitboards for attacks, masks, etc.
	Bitboard b1, b2, b3;

	//Squares that our king cannot move to
	Bitboard danger = 0;

	//For each enemy piece, add all of its attacks to the danger bitboard
	danger |= pawn_attacks<Them>(bitboard_of(Them, PAWN)) | attacks<KING>(their_king, all);

	b1 = bitboard_of(Them, KNIGHT);
	while (b1) danger |= attacks<KNIGHT>(pop_lsb(&b1), all);

	b1 = their_diag_sliders;
	//all ^ SQUARE_BB[our_king] is written to prevent the king from moving to squares which are 'x-rayed'
	//by enemy bishops and queens
	while (b1) danger |= attacks<BISHOP>(pop_lsb(&b1), all ^ SQUARE_BB[our_king]);

	b1 = their_orth_sliders;
	//all ^ SQUARE_BB[our_king] is written to prevent the king from moving to squares which are 'x-rayed'
	//by enemy rooks and queens
	while (b1) danger |= attacks<ROOK>(pop_lsb(&b1), all ^ SQUARE_BB[our_king]);

	//The king can move to all of its surrounding squares, except ones that are attacked, and
	//ones that have our own pieces on them
	b1 = attacks<KING>(our_king, all) & ~(us_bb | danger);
	//list = make<QUIET>(our_king, b1 & ~them_bb, list);
	list = make<CAPTURE>(our_king, b1 & them_bb, list);

	//The capture mask filters destination squares to those that contain an enemy piece that is checking the
	//king and must be captured
	Bitboard capture_mask;

	//The quiet mask filter destination squares to those where pieces must be moved to block an incoming attack
	//to the king
	Bitboard quiet_mask;

	//A general purpose square for storing destinations, etc.
	Square s;

	//Checkers of each piece type are identified by:
	//1. Projecting attacks FROM the king square
	//2. Intersecting this bitboard with the enemy bitboard of that piece type
	checkers = (attacks<KNIGHT>(our_king, all) & bitboard_of(Them, KNIGHT))
			   | (pawn_attacks<Us>(our_king) & bitboard_of(Them, PAWN));

	//Here, we identify slider checkers and pinners simultaneously, and candidates for such pinners
	//and checkers are represented by the bitboard <candidates>
	Bitboard candidates = (attacks<ROOK>(our_king, them_bb) & their_orth_sliders)
						  | (attacks<BISHOP>(our_king, them_bb) & their_diag_sliders);

	pinned = 0;
	while (candidates) {
		s = pop_lsb(&candidates);
		b1 = SQUARES_BETWEEN_BB[our_king][s] & us_bb;

		//Do the squares in between the enemy slider and our king contain any of our pieces?
		//If not, add the slider to the checker bitboard
		if (b1 == 0) checkers ^= SQUARE_BB[s];
			//If there is only one of our pieces between them, add our piece to the pinned bitboard
		else if ((b1 & b1 - 1) == 0) pinned ^= b1;
	}

	//This makes it easier to mask pieces
	const Bitboard not_pinned = ~pinned;

	switch (sparse_pop_count(checkers)) {
		case 2:
			//If there is a double check, the only legal moves are king moves out of check
			return list;
		case 1: {
			//It's a single check!

			Square checker_square = bsf(checkers);

			switch (board[checker_square]) {
				case make_piece(Them, PAWN):
					//If the checker is a pawn, we must check for e.p. moves that can capture it
					//This evaluates to true if the checking piece is the one which just double pushed
					if (checkers == shift<relative_dir<Us>(SOUTH)>(SQUARE_BB[history[game_ply].epsq])) {
						//b1 contains our pawns that can capture the checker e.p.
						b1 = pawn_attacks<Them>(history[game_ply].epsq) & bitboard_of(Us, PAWN) & not_pinned;
						while (b1) *list++ = Move(pop_lsb(&b1), history[game_ply].epsq, EN_PASSANT);
					}
					//FALL THROUGH INTENTIONAL
				case make_piece(Them, KNIGHT):
					//If the checker is either a pawn or a knight, the only legal moves are to capture
					//the checker. Only non-pinned pieces can capture it
					b1 = attackers_from<Us>(checker_square, all) & not_pinned;
					while (b1) {
						s = pop_lsb(&b1);
						if (type_of(board[s]) == PAWN && rank_of(s) == relative_rank<Us>(RANK7)) {
							*list++ = Move(s, checker_square, PC_QUEEN);
							*list++ = Move(s, checker_square, PC_ROOK);
							*list++ = Move(s, checker_square, PC_BISHOP);
							*list++ = Move(s, checker_square, PC_KNIGHT);
						}
						else *list++ = Move(s, checker_square, CAPTURE);
					}

					return list;
				default:
					//We must capture the checking piece
					capture_mask = checkers;

					//...or we can block it since it is guaranteed to be a slider
					quiet_mask = SQUARES_BETWEEN_BB[our_king][checker_square];
					break;
			}

			break;
		}

		default:
			//We can capture any enemy piece
			capture_mask = them_bb;

			//...and we can play a quiet move to any square which is not occupied
			quiet_mask = ~all;

			if (history[game_ply].epsq != NO_SQUARE) {
				b2 = pawn_attacks<Them>(history[game_ply].epsq) & bitboard_of(Us, PAWN);
				b1 = b2 & not_pinned;
				while (b1) {
					s = pop_lsb(&b1);
					if ((sliding_attacks(our_king, all ^ SQUARE_BB[s]
												   ^ shift<relative_dir<Us>(SOUTH)>(SQUARE_BB[history[game_ply].epsq]),
										 MASK_RANK[rank_of(our_king)]) &
						 their_orth_sliders) == 0)
						*list++ = Move(s, history[game_ply].epsq, EN_PASSANT);
				}

				//Pinned pawns can only capture e.p. if they are pinned diagonally and the e.p. square is in line with the king
				b1 = b2 & pinned & LINE[history[game_ply].epsq][our_king];
				if (b1) {
					*list++ = Move(bsf(b1), history[game_ply].epsq, EN_PASSANT);
				}
			}

			//For each pinned rook, bishop or queen...
			b1 = ~(not_pinned | bitboard_of(Us, KNIGHT));
			while (b1) {
				s = pop_lsb(&b1);

				//...only include attacks that are aligned with our king, since pinned pieces
				//are constrained to move in this direction only
				b2 = attacks(type_of(board[s]), s, all) & LINE[our_king][s];
				list = make<CAPTURE>(s, b2 & capture_mask, list);
			}

			//For each pinned pawn...
			b1 = ~not_pinned & bitboard_of(Us, PAWN);
			while (b1) {
				s = pop_lsb(&b1);

				if (rank_of(s) == relative_rank<Us>(RANK7)) {
					b2 = pawn_attacks<Us>(s) & capture_mask & LINE[our_king][s];
					list = make<PROMOTION_CAPTURES>(s, b2, list);
				}
				else {
					b2 = pawn_attacks<Us>(s) & them_bb & LINE[s][our_king];
					list = make<CAPTURE>(s, b2, list);
				}
			}
			break;
	}

	//Non-pinned knight moves
	b1 = bitboard_of(Us, KNIGHT) & not_pinned;
	while (b1) {
		s = pop_lsb(&b1);
		b2 = attacks<KNIGHT>(s, all);
		list = make<CAPTURE>(s, b2 & capture_mask, list);
	}

	//Non-pinned bishops and queens
	b1 = our_diag_sliders & not_pinned;
	while (b1) {
		s = pop_lsb(&b1);
		b2 = attacks<BISHOP>(s, all);
		list = make<CAPTURE>(s, b2 & capture_mask, list);
	}

	//Non-pinned rooks and queens
	b1 = our_orth_sliders & not_pinned;
	while (b1) {
		s = pop_lsb(&b1);
		b2 = attacks<ROOK>(s, all);
		list = make<CAPTURE>(s, b2 & capture_mask, list);
	}

	//b1 contains non-pinned pawns which are not on the last rank
	b1 = bitboard_of(Us, PAWN) & not_pinned & ~MASK_RANK[relative_rank<Us>(RANK7)];

	//Single pawn pushes
	b2 = shift<relative_dir<Us>(NORTH)>(b1) & ~all;

	//Double pawn pushes (only pawns on rank 3/6 are eligible)
	b3 = shift<relative_dir<Us>(NORTH)>(b2 & MASK_RANK[relative_rank<Us>(RANK3)]) & quiet_mask;

	//We & this with the quiet mask only later, as a non-check-blocking single push does NOT mean that the
	//corresponding double push is not blocking check either.
	b2 &= quiet_mask;

	//Pawn captures
	b2 = shift<relative_dir<Us>(NORTH_WEST)>(b1) & capture_mask;
	b3 = shift<relative_dir<Us>(NORTH_EAST)>(b1) & capture_mask;

	while (b2) {
		s = pop_lsb(&b2);
		*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, CAPTURE);
	}

	while (b3) {
		s = pop_lsb(&b3);
		*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, CAPTURE);
	}

	//b1 now contains non-pinned pawns which ARE on the last rank (about to promote)
	b1 = bitboard_of(Us, PAWN) & not_pinned & MASK_RANK[relative_rank<Us>(RANK7)];
	if (b1) {
		//Quiet promotions
		b2 = shift<relative_dir<Us>(NORTH_WEST)>(b1) & capture_mask;
		b3 = shift<relative_dir<Us>(NORTH_EAST)>(b1) & capture_mask;

		while (b2) {
			s = pop_lsb(&b2);
			//One move is added for each promotion piece
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_KNIGHT);
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_BISHOP);
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_ROOK);
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_QUEEN);
		}

		while (b3) {
			s = pop_lsb(&b3);
			//One move is added for each promotion piece
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_KNIGHT);
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_BISHOP);
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_ROOK);
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_QUEEN);
		}
	}

	return list;
}

//Generates all legal moves in a position for the given side. Advances the move pointer and returns it.
//TODO: separate logic out for each MoveGenerationOption
template<Color Us>
Move* Position::generate_legals(Move* list, const MoveGenerationOptions &options) {
	if (options.generate_captures && !options.generate_quiet && !options.generate_promotion && !options.generate_checks) {
		return generate_captures<Us>(list);
	}
	constexpr Color Them = ~Us;

	const Bitboard us_bb = all_pieces<Us>();
	const Bitboard them_bb = all_pieces<Them>();
	const Bitboard all = us_bb | them_bb;

	const Square our_king = bsf(bitboard_of(Us, KING));
	const Square their_king = bsf(bitboard_of(Them, KING));

	const Bitboard our_diag_sliders = diagonal_sliders<Us>();
	const Bitboard their_diag_sliders = diagonal_sliders<Them>();
	const Bitboard our_orth_sliders = orthogonal_sliders<Us>();
	const Bitboard their_orth_sliders = orthogonal_sliders<Them>();

	//General purpose bitboards for attacks, masks, etc.
	Bitboard b1, b2, b3;

	//Squares that our king cannot move to
	Bitboard danger = 0;

	//For each enemy piece, add all of its attacks to the danger bitboard
	danger |= pawn_attacks<Them>(bitboard_of(Them, PAWN)) | attacks<KING>(their_king, all);
	
	b1 = bitboard_of(Them, KNIGHT); 
	while (b1) danger |= attacks<KNIGHT>(pop_lsb(&b1), all);
	
	b1 = their_diag_sliders;
	//all ^ SQUARE_BB[our_king] is written to prevent the king from moving to squares which are 'x-rayed'
	//by enemy bishops and queens
	while (b1) danger |= attacks<BISHOP>(pop_lsb(&b1), all ^ SQUARE_BB[our_king]);
	
	b1 = their_orth_sliders;
	//all ^ SQUARE_BB[our_king] is written to prevent the king from moving to squares which are 'x-rayed'
	//by enemy rooks and queens
	while (b1) danger |= attacks<ROOK>(pop_lsb(&b1), all ^ SQUARE_BB[our_king]);

	//The king can move to all of its surrounding squares, except ones that are attacked, and
	//ones that have our own pieces on them
	b1 = attacks<KING>(our_king, all) & ~(us_bb | danger);
	list = make<QUIET>(our_king, b1 & ~them_bb, list);
	list = make<CAPTURE>(our_king, b1 & them_bb, list);

	//The capture mask filters destination squares to those that contain an enemy piece that is checking the 
	//king and must be captured
	Bitboard capture_mask;
	
	//The quiet mask filter destination squares to those where pieces must be moved to block an incoming attack 
	//to the king
	Bitboard quiet_mask;
	
	//A general purpose square for storing destinations, etc.
	Square s;

	//Checkers of each piece type are identified by:
	//1. Projecting attacks FROM the king square
	//2. Intersecting this bitboard with the enemy bitboard of that piece type
	checkers = (attacks<KNIGHT>(our_king, all) & bitboard_of(Them, KNIGHT))
		| (pawn_attacks<Us>(our_king) & bitboard_of(Them, PAWN));
	
	//Here, we identify slider checkers and pinners simultaneously, and candidates for such pinners 
	//and checkers are represented by the bitboard <candidates>
	Bitboard candidates = (attacks<ROOK>(our_king, them_bb) & their_orth_sliders)
		| (attacks<BISHOP>(our_king, them_bb) & their_diag_sliders);

	pinned = 0;
	while (candidates) {
		s = pop_lsb(&candidates);
		b1 = SQUARES_BETWEEN_BB[our_king][s] & us_bb;
		
		//Do the squares in between the enemy slider and our king contain any of our pieces?
		//If not, add the slider to the checker bitboard
		if (b1 == 0) checkers ^= SQUARE_BB[s];
		//If there is only one of our pieces between them, add our piece to the pinned bitboard 
		else if ((b1 & b1 - 1) == 0) pinned ^= b1;
	}

	//This makes it easier to mask pieces
	const Bitboard not_pinned = ~pinned;

	switch (sparse_pop_count(checkers)) {
	case 2:
		//If there is a double check, the only legal moves are king moves out of check
		return list;
	case 1: {
		//It's a single check!

		Square checker_square = bsf(checkers);

		switch (board[checker_square]) {
		case make_piece(Them, PAWN):
			//If the checker is a pawn, we must check for e.p. moves that can capture it
			//This evaluates to true if the checking piece is the one which just double pushed
			if (checkers == shift<relative_dir<Us>(SOUTH)>(SQUARE_BB[history[game_ply].epsq])) {
				//b1 contains our pawns that can capture the checker e.p.
				b1 = pawn_attacks<Them>(history[game_ply].epsq) & bitboard_of(Us, PAWN) & not_pinned;
				while (b1) *list++ = Move(pop_lsb(&b1), history[game_ply].epsq, EN_PASSANT);
			}
			//FALL THROUGH INTENTIONAL
		case make_piece(Them, KNIGHT):
			//If the checker is either a pawn or a knight, the only legal moves are to capture
			//the checker. Only non-pinned pieces can capture it
			b1 = attackers_from<Us>(checker_square, all) & not_pinned;
			while (b1) {
				s = pop_lsb(&b1);
				if (type_of(board[s]) == PAWN && rank_of(s) == relative_rank<Us>(RANK7)) {
					*list++ = Move(s, checker_square, PC_QUEEN);
					*list++ = Move(s, checker_square, PC_ROOK);
					*list++ = Move(s, checker_square, PC_BISHOP);
					*list++ = Move(s, checker_square, PC_KNIGHT);
				}
				else *list++ = Move(s, checker_square, CAPTURE);
			}

			return list;
		default:
			//We must capture the checking piece
			capture_mask = checkers;

			//...or we can block it since it is guaranteed to be a slider
			quiet_mask = SQUARES_BETWEEN_BB[our_king][checker_square];
			break;
		}

		break;
	}

	default:
		//We can capture any enemy piece
		capture_mask = them_bb;

		//...and we can play a quiet move to any square which is not occupied
		quiet_mask = ~all;

		if (history[game_ply].epsq != NO_SQUARE) {
			//b1 contains our pawns that can perform an e.p. capture
			b2 = pawn_attacks<Them>(history[game_ply].epsq) & bitboard_of(Us, PAWN);
			b1 = b2 & not_pinned;
			while (b1) {
				s = pop_lsb(&b1);

				//This piece of evil bit-fiddling magic prevents the infamous 'pseudo-pinned' e.p. case,
				//where the pawn is not directly pinned, but on moving the pawn and capturing the enemy pawn
				//e.p., a rook or queen attack to the king is revealed

				/*
				.nbqkbnr
				ppp.pppp
				........
				r..pP..K
				........
				........
				PPPP.PPP
				RNBQ.BNR

				Here, if white plays exd5 e.p., the black rook on a5 attacks the white king on h5
				*/

				if ((sliding_attacks(our_king, all ^ SQUARE_BB[s]
					^ shift<relative_dir<Us>(SOUTH)>(SQUARE_BB[history[game_ply].epsq]),
					MASK_RANK[rank_of(our_king)]) &
					their_orth_sliders) == 0)
						*list++ = Move(s, history[game_ply].epsq, EN_PASSANT);
			}

			//Pinned pawns can only capture e.p. if they are pinned diagonally and the e.p. square is in line with the king
			b1 = b2 & pinned & LINE[history[game_ply].epsq][our_king];
			if (b1) {
				*list++ = Move(bsf(b1), history[game_ply].epsq, EN_PASSANT);
			}
		}

		//Only add castling if:
		//1. The king and the rook have both not moved
		//2. No piece is attacking between the the rook and the king
		//3. The king is not in check
		if (!((history[game_ply].entry & oo_mask<Us>()) | ((all | danger) & oo_blockers_mask<Us>())))
			*list++ = Us == WHITE ? Move(e1, g1, OO) : Move(e8, g8, OO);
		if (!((history[game_ply].entry & ooo_mask<Us>()) |
			((all | (danger & ~ignore_ooo_danger<Us>())) & ooo_blockers_mask<Us>())))
			*list++ = Us == WHITE ? Move(e1, c1, OOO) : Move(e8, c8, OOO);

		//For each pinned rook, bishop or queen...
		b1 = ~(not_pinned | bitboard_of(Us, KNIGHT));
		while (b1) {
			s = pop_lsb(&b1);

			//...only include attacks that are aligned with our king, since pinned pieces
			//are constrained to move in this direction only
			b2 = attacks(type_of(board[s]), s, all) & LINE[our_king][s];
			list = make<QUIET>(s, b2 & quiet_mask, list);
			list = make<CAPTURE>(s, b2 & capture_mask, list);
		}

		//For each pinned pawn...
		b1 = ~not_pinned & bitboard_of(Us, PAWN);
		while (b1) {
			s = pop_lsb(&b1);

			if (rank_of(s) == relative_rank<Us>(RANK7)) {
				//Quiet promotions are impossible since the square in front of the pawn will
				//either be occupied by the king or the pinner, or doing so would leave our king
				//in check
				b2 = pawn_attacks<Us>(s) & capture_mask & LINE[our_king][s];
				list = make<PROMOTION_CAPTURES>(s, b2, list);
			}
			else {
				b2 = pawn_attacks<Us>(s) & them_bb & LINE[s][our_king];
				list = make<CAPTURE>(s, b2, list);

				//Single pawn pushes
				b2 = shift<relative_dir<Us>(NORTH)>(SQUARE_BB[s]) & ~all & LINE[our_king][s];
				//Double pawn pushes (only pawns on rank 3/6 are eligible)
				b3 = shift<relative_dir<Us>(NORTH)>(b2 &
					MASK_RANK[relative_rank<Us>(RANK3)]) & ~all & LINE[our_king][s];
				list = make<QUIET>(s, b2, list);
				list = make<DOUBLE_PUSH>(s, b3, list);
			}
		}

		//Pinned knights cannot move anywhere, so we're done with pinned pieces!

		break;
	}

	//Non-pinned knight moves
	b1 = bitboard_of(Us, KNIGHT) & not_pinned;
	while (b1) {
		s = pop_lsb(&b1);
		b2 = attacks<KNIGHT>(s, all);
		list = make<QUIET>(s, b2 & quiet_mask, list);
		list = make<CAPTURE>(s, b2 & capture_mask, list);
	}

	//Non-pinned bishops and queens
	b1 = our_diag_sliders & not_pinned;
	while (b1) {
		s = pop_lsb(&b1);
		b2 = attacks<BISHOP>(s, all);
		list = make<QUIET>(s, b2 & quiet_mask, list);
		list = make<CAPTURE>(s, b2 & capture_mask, list);
	}

	//Non-pinned rooks and queens
	b1 = our_orth_sliders & not_pinned;
	while (b1) {
		s = pop_lsb(&b1);
		b2 = attacks<ROOK>(s, all);
		list = make<QUIET>(s, b2 & quiet_mask, list);
		list = make<CAPTURE>(s, b2 & capture_mask, list);
	}

	//b1 contains non-pinned pawns which are not on the last rank
	b1 = bitboard_of(Us, PAWN) & not_pinned & ~MASK_RANK[relative_rank<Us>(RANK7)];

	//Single pawn pushes
	b2 = shift<relative_dir<Us>(NORTH)>(b1) & ~all;

	//Double pawn pushes (only pawns on rank 3/6 are eligible)
	b3 = shift<relative_dir<Us>(NORTH)>(b2 & MASK_RANK[relative_rank<Us>(RANK3)]) & quiet_mask;

	//We & this with the quiet mask only later, as a non-check-blocking single push does NOT mean that the
	//corresponding double push is not blocking check either.
	b2 &= quiet_mask;

	while (b2) {
		s = pop_lsb(&b2);
		*list++ = Move(s - relative_dir<Us>(NORTH), s, QUIET);
	}

	while (b3) {
		s = pop_lsb(&b3);
		*list++ = Move(s - relative_dir<Us>(NORTH_NORTH), s, DOUBLE_PUSH);
	}

	//Pawn captures
	b2 = shift<relative_dir<Us>(NORTH_WEST)>(b1) & capture_mask;
	b3 = shift<relative_dir<Us>(NORTH_EAST)>(b1) & capture_mask;

	while (b2) {
		s = pop_lsb(&b2);
		*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, CAPTURE);
	}

	while (b3) {
		s = pop_lsb(&b3);
		*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, CAPTURE);
	}

	//b1 now contains non-pinned pawns which ARE on the last rank (about to promote)
	b1 = bitboard_of(Us, PAWN) & not_pinned & MASK_RANK[relative_rank<Us>(RANK7)];
	if (b1) {
		//Quiet promotions
		b2 = shift<relative_dir<Us>(NORTH)>(b1) & quiet_mask;
		while (b2) {
			s = pop_lsb(&b2);
			//One move is added for each promotion piece
			*list++ = Move(s - relative_dir<Us>(NORTH), s, PR_KNIGHT);
			*list++ = Move(s - relative_dir<Us>(NORTH), s, PR_BISHOP);
			*list++ = Move(s - relative_dir<Us>(NORTH), s, PR_ROOK);
			*list++ = Move(s - relative_dir<Us>(NORTH), s, PR_QUEEN);
		}

		//Promotion captures
		b2 = shift<relative_dir<Us>(NORTH_WEST)>(b1) & capture_mask;
		b3 = shift<relative_dir<Us>(NORTH_EAST)>(b1) & capture_mask;

		while (b2) {
			s = pop_lsb(&b2);
			//One move is added for each promotion piece
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_KNIGHT);
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_BISHOP);
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_ROOK);
			*list++ = Move(s - relative_dir<Us>(NORTH_WEST), s, PC_QUEEN);
		}

		while (b3) {
			s = pop_lsb(&b3);
			//One move is added for each promotion piece
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_KNIGHT);
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_BISHOP);
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_ROOK);
			*list++ = Move(s - relative_dir<Us>(NORTH_EAST), s, PC_QUEEN);
		}
	}

	return list;
}

//A convenience class for interfacing with legal moves, rather than using the low-level
//generate_legals() function directly. It can be iterated over.
template<Color Us>
class MoveList {
public:
	const MoveGenerationOptions DefaultMoveGenerationOptions = {
		.generate_captures = true,
		.generate_checks = true,
		.generate_promotion = true,
		.generate_quiet = true,
	};
	explicit MoveList(Position& p) : last(p.generate_legals<Us>(list, DefaultMoveGenerationOptions)) {}
	explicit MoveList(Position& p, const MoveGenerationOptions& options) : last(p.generate_legals<Us>(list, options)) {}

	const Move* begin() const { return list; }
	const Move* end() const { return last; }
	size_t size() const { return last - list; }
private:
    Move list[218];
	Move *last;
};
