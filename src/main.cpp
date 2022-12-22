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

/*
int main() {
    Position unchanged, changed;
    const std::string& startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1";
    const std::string& randFen = "8/8/1k6/8/1N6/5K2/8/r7 b -  -";
    Position::set(startFen, unchanged);

    Position::set(startFen, changed);
    Position::set("rnb2knr/p3p2p/3p2p1/3q1p2/p1p2B2/2PP1PPP/1P1KP1B1/RN2Q1NR b -  -", changed);
    Position::set("rnb3nr/p3pk1p/3p2p1/3q1p2/p1p2B2/2PP1PPP/1P1KP1B1/RN1Q2NR b -  -", changed);
    Position::set(randFen, changed);
    Position::set(startFen, changed);
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
            for (int i = 24; i < Line.size(); i += 5) {
                Move nextMove(Line.substr(i, 4));
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