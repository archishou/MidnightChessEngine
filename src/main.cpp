#include <algorithm>
#include <iostream>
#include "move_generation/tables.h"
#include "move_generation/position.h"
#include "move_generation/types.h"
#include "move_search/search.h"

template<Color Us>
unsigned long long perft(Position& p, unsigned int depth) {
    int nmoves;
    unsigned long long nodes = 0;

    MoveList<Us> list(p);

    if (depth == 1) return (unsigned long long) list.size();

    for (Move move : list) {
        p.play<Us>(move);
        nodes += perft<~Us>(p, depth - 1);
        p.undo<Us>(move);
    }

    return nodes;
}

void test_perft() {
    Position p;
    const std::string& fen = "8/8/1k6/8/1N6/5K2/8/r7 b -  -";
    Position::set(fen, p);
    std::cout << p;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto n = perft<WHITE>(p, 1);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto diff = end - begin;

    std::cout << "Nodes: " << n << "\n";
    std::cout << "NPS: "
              << int(n * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(diff).count())
              << "\n";
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " [microseconds]\n";
}
using namespace std;

Move uciToMove(const std::string& moveStr, Position& position) {
    Move move = Move(moveStr.substr(0, 4));
    // Pawn Promotion
    if (moveStr.size() == 5) {
		/*
		Could be dangerous if order in enum is changed!
		int promotionType = position.at(move.to()) == NO_PIECE ? PR_KNIGHT : PC_KNIGHT;
		if (moveStr.at(4) == 'n') return Move(move.from(), move.to(), (MoveFlags) promotionType);
		if (moveStr.at(4) == 'b') return Move(move.from(), move.to(), (MoveFlags) (promotionType + 1));
		if (moveStr.at(4) == 'r') return Move(move.from(), move.to(), (MoveFlags) (promotionType + 2));
		if (moveStr.at(4) == 'q') return Move(move.from(), move.to(), (MoveFlags) (promotionType + 3));
		*/
        // Quiet Promotion
        if (position.at(move.to()) == NO_PIECE) {
            if (moveStr.at(4) == 'q') return Move(move.from(), move.to(), PR_QUEEN);
            if (moveStr.at(4) == 'b') return Move(move.from(), move.to(), PR_BISHOP);
            if (moveStr.at(4) == 'n') return Move(move.from(), move.to(), PR_KNIGHT);
            if (moveStr.at(4) == 'r') return Move(move.from(), move.to(), PR_ROOK);
        }
        if (moveStr.at(4) == 'q') return Move(move.from(), move.to(), PC_QUEEN);
        if (moveStr.at(4) == 'b') return Move(move.from(), move.to(), PC_BISHOP);
        if (moveStr.at(4) == 'n') return Move(move.from(), move.to(), PC_KNIGHT);
        if (moveStr.at(4) == 'r') return Move(move.from(), move.to(), PC_ROOK);
    }

    // En Passant
    if (position.at(move.to()) == NO_PIECE && type_of(position.at(move.from())) == PAWN &&
        file_of(move.to()) != file_of(move.from())) {
        return Move(move.from(), move.to(), EN_PASSANT);
    }

    if (type_of(position.at(move.from())) == PAWN && rank_of(move.to()) - rank_of(move.from()) == 2) {
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
vector<string> split(const string& s, const string& delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
}
/*
int main() {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();
	const std::string& testFen = "r1bqkb1r/1ppppppp/p7/3PP3/5Bn1/2NQ1N2/PPP2PPP/R3K2R b KQkq -";
	Position board;
	Position::set(testFen, board);

	std::cout << board << std::endl;
	Move best = bestMove<BLACK>(board);
	std::cout << best << std::endl;

	const Color startTeam = BLACK;
	int depth = 6;
	int i  = 0;
	while (i < depth) {
		Move ourBest = alphaBetaRoot<startTeam>(board, depth - i);
		std::cout << ourBest << std::endl;
		board.play<startTeam>(ourBest);
		i += 1;
		Move thierBest = alphaBetaRoot<~startTeam>(board, depth - i);
		std::cout << thierBest << std::endl;
		board.play<~startTeam>(thierBest);
		i += 1;
	}

}
*/
int main () {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();
    Position p;
    const std::string& startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position::set(startFen, p);

    string Line; //to read the command given by the GUI
    int flag = 2; //to change the value of chess squares from 'a' to 'h'

    cout.setf (ios::unitbuf);// Make sure that the outputs are sent straight away to the GUI

    ofstream myfile;
    myfile.open ("/Users/Archish/Documents/CodeProjects/C/ChessEngine/src/example.txt");
    while (getline(cin, Line)) {
        myfile << Line << std::endl;
        if (Line == "uci") {
            cout << "id name Demo_engine" << endl;
            cout << "id author XXX" << endl;
            cout << "uciok" << endl;
        } else if (Line == "quit") {
            cout << "Bye Bye" << endl;
            break;
        } else if (Line == "isready") {
            cout << "readyok" << endl;
        } else if (Line == "ucinewgame") {
            ; // nothing to do
        }
        if (Line.substr(0,23) == "position startpos moves") {
            Position::set(startFen, p);
			vector<string> uciMoves = split(Line.substr(24, Line.size() - 24), " ");
            for (const std::string& uciMove : uciMoves) {
                Move nextMove = uciToMove(uciMove, p);
                if (p.turn() == BLACK) p.play<BLACK>(nextMove);
                else p.play<WHITE>(nextMove);
            }
        } else if (Line == "stop") {
            ; // nothing to do
        } else if (Line.substr( 0, 2 ) == "go") {
            // Received a command like: "go wtime 300000 btime 300000 winc 0 binc 0"
            Move move;
            if (p.turn() == BLACK) move = bestMove<BLACK>(p);
            else move = bestMove<WHITE>(p);
            myfile << "Predicted Best Move: " << move << ":" << p.ply() << std::endl;
            cout << "bestmove " << move << endl;
            //Output like: "bestmove h7h5"
            flag++; //increase flag to move other pawn on next turn
        }
    }
    myfile.close();
    return 0;
}