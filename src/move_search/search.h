//
// Created by Archishmaan on 12/21/22.
//
#include <vector>
#include <random>
#include "position.h"
template<Color color>
Move bestMove(Position board) {
    /*
    std::random_device dev;
    std::mt19937 rng(dev());
    MoveList<color> moveList(board);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, moveList.size() - 1);
    int idx = dist(rng);
    Move move = *(moveList.begin() + idx);
    return move;
     */
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
