//
// Created by Archishmaan on 12/21/22.
//

#include "position.h"
template<Color color>
Move evaluate(Position& board) {

	board.all_pieces<color>(board);
}
