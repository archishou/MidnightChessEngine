#include <iostream>
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
    Position::set("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq -", p);
    std::cout << p;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto n = perft<WHITE>(p, 6);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto diff = end - begin;

    std::cout << "Nodes: " << n << "\n";
    std::cout << "NPS: "
              << int(n * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(diff).count())
              << "\n";
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " [microseconds]\n";
}

int main() {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();
    Position p;
    Position::set("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq -", p);
    std::cout << p;

    std::string userMove;
    std::cin >> userMove;

    while (userMove != "quit") {

        MoveList<WHITE> moveList(p);
        int moveListLength = sizeof(moveList.list) / sizeof(moveList.list[0]);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0,moveListLength);

        int randomIdx = dist6(rng);

        std::cout << randomIdx << std::endl;
        std::cout << moveList.list[randomIdx];
    }
}
