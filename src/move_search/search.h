//
// Created by Archishmaan on 12/21/22.
//
#include <vector>
#include <random>
#include "position.h"

template<Color color>
Move bestMove(Position board) {
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
