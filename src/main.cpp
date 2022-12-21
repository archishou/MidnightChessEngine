#include <iostream>
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <random>
#include "move_generation/tables.h"
#include "move_generation/position.h"
#include "move_generation/types.h"

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

//A variant of perft, listing all moves and for each move, the perft of the decremented depth
//It is used solely for debugging
template<Color Us>
void perftdiv(Position& p, unsigned int depth) {
    unsigned long long nodes = 0, pf;

    MoveList<Us> list(p);

    for (Move move : list) {
        std::cout << move;

        p.play<Us>(move);
        pf = perft<~Us>(p, depth - 1);
        std::cout << ": " << pf << " moves\n";
        nodes += pf;
        p.undo<Us>(move);
    }

    std::cout << "\nTotal: " << nodes << " moves\n";
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

template <Color color>
Move randomMove(Position board) {
    MoveList<color> moveList(board);
    std::vector<Move> out;
    size_t nelems = 1;
    std::sample(
            moveList.begin(),
            moveList.end(),
            std::back_inserter(out),
            nelems,
            std::mt19937{std::random_device{}()}
    );
    return out[0];
}
template <Color color>
int checkmate(Position board) {
    std::cout << "here 2" << std::endl;
    MoveList<color> moveList(board);
    std::cout << "here 3" << std::endl;
    return moveList.size() == 0;
}

int main() {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();
    const std::string& startFen ="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ;
    Position board;
    Position::set(startFen, board);
    std::cout << board;

    int totalMovesMade = 0;
    while (true) {
        std::cout << "here 0" << std::endl;
        if (!checkmate<WHITE>(board)) {
            std::cout << "here 4" << std::endl;
            Move move = randomMove<WHITE>(board);
            std::cout << "here 5" << std::endl;
            board.play<WHITE>(move);
            totalMovesMade += 1;
            std::cout << board << totalMovesMade << std::endl;
        } else {break;}
        std::cout << "here 1" << std::endl;
        if (!checkmate<BLACK>(board)) {
            std::cout << "here 6" << std::endl;
            Move move = randomMove<BLACK>(board);
            std::cout << "here 7" << std::endl;
            board.play<BLACK>(move);
            totalMovesMade += 1;

            std::cout << board << totalMovesMade << std::endl;
        } else {break;}
        std::cout << "here 10" << std::endl;
    }
    std::cout << "COMPLETE" << std::endl;

    return 0;
}

/*int main() {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();
    test_perft();
}*/

/*using namespace std;

int main () {
    const Color team = WHITE;
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();
    Position p;
    Position::set("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq -", p);

    string Line; //to read the command given by the GUI
    int flag = 2; //to change the value of chess squares from 'a' to 'h'

    cout.setf (ios::unitbuf);// Make sure that the outputs are sent straight away to the GUI

    while( getline( cin, Line ) ) {
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

        if ( Line.substr(0,23) == "position startpos moves " ) {
            ; // nothing to do
        } else if ( Line == "stop" ) {
            ; // nothing to do
        } else if ( Line.substr( 0, 2 ) == "go" ) {
            // Received a command like: "go wtime 300000 btime 300000 winc 0 binc 0"

            MoveList<team> moveList(p);
            int moveListLength = sizeof(moveList.list) / sizeof(moveList.list[0]);

            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(0,moveListLength);

            Move randMove = moveList.list[dist6(rng)];
            p.play<team>(randMove);
            cout << "bestmove " << randMove << endl;
            //Output like: "bestmove h7h5"
            flag++; //increase flag to move other pawn on next turn
        }
    }

    return 0;
}
 */