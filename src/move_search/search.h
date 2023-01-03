//
// Created by Archishmaan on 12/21/22.
//
#include <fstream>
#include <vector>
#include <random>
#include "position.h"
#include "id.h"

template<Color color>
Move bestMove(Position& board) {
	return iterativeDeepening<color>(board, DEPTH).bestMove;
}
