//
// Created by Archishmaan Peyyety on 2/1/23.
//
#pragma once
#include "../board/position.h"
#include "../utils/helpers.h"
#include <locale>

char promotion_character(string uci_move);
Move uci_to_move(const string& moveStr, Position& position);
void uci_update_position_from_moves(Position& board, const string& uci_move_string);
