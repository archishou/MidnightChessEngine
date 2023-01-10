#include "ChessEngine_lib/uci_interpreter/uci_interpreter.h"
#include "move_generation/types.h"
#include "move_generation/position.h"

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
    const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position::set(fen, p);
    std::cout << p;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto n = perft<WHITE>(p, 4);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto diff = end - begin;

    std::cout << "Nodes: " << n << "\n";
    std::cout << "NPS: "
              << int(n * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(diff).count())
              << "\n";
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " [microseconds]\n";
}
int main () {
	initialise_all_databases();
	zobrist::initialise_zobrist_keys();
	test_perft();
	//read_uci("/Users/archishmaan/Documents/CodeProjects/C/ChessEngine/src/engine_uci_inputs_diagnostics.txt");
}
