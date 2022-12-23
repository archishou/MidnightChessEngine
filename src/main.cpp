#include <algorithm>
#include <iostream>
#include <fstream>
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
vector<string> split (string s, string delimiter) {
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

    const std::string& startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position p;
    Position::set(startFen, p);
    const std::string& Line = "position startpos moves b2b4 g7g5 g2g3 h7h6 g3g4 e7e5 c2c3 d7d5 b4b5 e8d7 h2h3 c7c6 e2e3 f7f6 b5c6 d7c7 c1a3 f8b4 c3b4 b8d7 f1b5 a7a6 d1a4 b7c6 b5d3 a8b8 d3f5 c7d6 e1e2 d8f8 e2d3 b8b4 b1c3 d6c5 a4a5 c5d6 c3b1 d6e7 f2f4 f8d8 d3c3 e7e8 a5a4 c8b7 a3c1 d8e7 a4b3 e7c5 c3d3 c5c3 d2c3 b4d4 d3c2 d4a4 c2b2 d7c5 f4g5 g8e7 g5g6 b7c8 e3e4 c8b7 b3d5 a4b4 b2c2 b7a8 d5g8 h8g8 f5c8 c5a4 c1f4 g8f8 c8a6 e5f4 c2d3 a8b7 g4g5 h6g5 a6b7 b4c4 b7c8 f4f3 d3e3 c6c5 c8a6 e8d8 b1d2 a4b6 g6g7 e7c6 g7f8r";
	std::cout << Line.substr(24, Line.size() - 24) << std::endl;
	vector<string> moves = split(Line.substr(24, Line.size() - 24), " ");

	for (const std::string& s : moves) {
		std::cout << s << std::endl;
	}

	for (const std::string& uciMove : moves) {
		std::cout << uciMove << std::endl;
        Move nextMove = uciToMove(uciMove, p);
        if (p.turn() == BLACK) {
            std::cout << MoveList<BLACK>(p).size() << std::endl;
            p.play<BLACK>(nextMove);
        }
        else {
            std::cout << MoveList<WHITE>(p).size() << std::endl;
            p.play<WHITE>(nextMove);
        }
    }
    if (p.turn() == BLACK) {
        for (Move m : MoveList<BLACK>(p)) std::cout << m << std::endl;
    }
    else {
        for (Move m : MoveList<WHITE>(p)) std::cout << m << std::endl;
    }
    std::cout << ":" << std::endl;
    Position check;
    Position::set(p.fen(), check);
    if (check.turn() == BLACK) {
        for (Move m : MoveList<BLACK>(check)) std::cout << m << std::endl;
    }
    else {
        for (Move m : MoveList<WHITE>(check)) std::cout << m << std::endl;
    }
    return 0;
}
 */
/*
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
    while( getline( cin, Line ) ) {
        myfile << Line << std::endl;
        if ( Line == "uci" ) {
            cout << "id name Demo_engine" << endl;
            cout << "id author XXX" << endl;
            cout << "uciok" << endl;
        } else if ( Line == "quit" ) {
            cout << "Bye Bye" << endl;
            break;
        } else if ( Line == "isready" ) {
            cout << "readyok" << endl;
        } else if ( Line == "ucinewgame" ) {
            ; // nothing to do
        }

        if ( Line.substr(0,23) == "position startpos moves") {
        } else if ( Line == "stop" ) {
            ; // nothing to do
        } else if ( Line.substr( 0, 2 ) == "go" ) {
            // Received a command like: "go wtime 300000 btime 300000 winc 0 binc 0"
            Move move;
            if (p.turn() == BLACK) {
                move = bestMove<BLACK>(p);
                p.play<BLACK>(move);
            } else {
                move = bestMove<WHITE>(p);
                p.play<WHITE>(move);
            }
            myfile << "Predicted Best Move: " << move << ":" << p.ply() << std::endl;
            cout << "bestmove " << move << endl;
            //Output like: "bestmove h7h5"
            flag++; //increase flag to move other pawn on next turn
        }
    }
    myfile.close();

    return 0;
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
    while( getline( cin, Line ) ) {
        myfile << Line << std::endl;
        if ( Line == "uci" ) {
            cout << "id name Demo_engine" << endl;
            cout << "id author XXX" << endl;
            cout << "uciok" << endl;
        } else if ( Line == "quit" ) {
            cout << "Bye Bye" << endl;
            break;
        } else if ( Line == "isready" ) {
            cout << "readyok" << endl;
        } else if ( Line == "ucinewgame" ) {
            ; // nothing to do
        }
        if ( Line.substr(0,23) == "position startpos moves") {
            Position::set(startFen, p);
			vector<string> uciMoves = split(Line.substr(24, Line.size() - 24), " ");
            for (const std::string& uciMove : uciMoves) {
                Move nextMove = uciToMove(uciMove, p);
                if (p.turn() == BLACK) p.play<BLACK>(nextMove);
                else p.play<WHITE>(nextMove);
            }
        } else if ( Line == "stop" ) {
            ; // nothing to do
        } else if ( Line.substr( 0, 2 ) == "go" ) {
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