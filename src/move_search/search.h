//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "position.h"

template<Color color>
Move bestMove(Position& board) {
    std::random_device dev;
    std::mt19937 rng(dev());
    MoveList<color> moveList(board);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, moveList.size() - 1);
    int idx = dist(rng);
    Move move = *(moveList.begin() + idx);
    return move;
}
