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
int main() {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();

    const std::string& startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position p;
    Position::set(startFen, p);
    const std::string& Line = "c2c4 h7h5 d2d4 h8h7 c1f4 c7c5 d4c5 g7g6 g1h3 d8a5 b1c3 a5c3 d1d2 c3c1";
    for (int i = 0; i < Line.size(); i += 5) {
        Move nextMove(Line.substr(i, 4));
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
        std::cout << MoveList<BLACK>(p).size() << std::endl;
    }
    else {
        std::cout << MoveList<WHITE>(p).size() << std::endl;
    }
    Position check;
    Position::set(p.fen(), check);
    if (check.turn() == BLACK) {
        std::cout << MoveList<BLACK>(check).size() << std::endl;
    }
    else {
        std::cout << MoveList<WHITE>(check).size() << std::endl;
    }
    return 0;
}
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
            Position::set(startFen, p);
            Position checker;
            Position::set(startFen, checker);
            myfile << "Equal? " << Position::equality(p, checker) << std::endl;
            for (int i = 24; i < Line.size(); i += 5) {
                Move nextMove(Line.substr(i, 4));
                if (p.turn() == BLACK) p.play<BLACK>(nextMove);
                else p.play<WHITE>(nextMove);
            }
        } else if ( Line == "stop" ) {
            ; // nothing to do
        } else if ( Line.substr( 0, 2 ) == "go" ) {
            // Received a command like: "go wtime 300000 btime 300000 winc 0 binc 0"

            myfile << "Board " << p << std::endl;
            Move move;

            if (p.turn() == BLACK) {
                std::random_device dev;
                std::mt19937 rng(dev());
                MoveList<BLACK> moveList(p);
                for (Move m : moveList) myfile << "Possible Move: " << m << std::endl;
                std::uniform_int_distribution<std::mt19937::result_type> dist(0, moveList.size() - 1);
                int idx = dist(rng);
                move = *(moveList.begin() + idx);
            }

            else {
                std::random_device dev;
                std::mt19937 rng(dev());
                MoveList<WHITE> moveList(p);
                for (Move m : moveList) myfile << "Possible Move: " << m << std::endl;
                std::uniform_int_distribution<std::mt19937::result_type> dist(0, moveList.size() - 1);
                int idx = dist(rng);
                move = *(moveList.begin() + idx);
            }

            myfile << "Predicted Best Move: " << move << ":" << p.ply() << std::endl;
            cout << "bestmove " << move << endl;
            //Output like: "bestmove h7h5"
            flag++; //increase flag to move other pawn on next turn
        }
    }
    myfile.close();

    return 0;
}*/