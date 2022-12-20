#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "board.h"

std::vector<std::string> split(std::string str, char sep)
{
    std::vector<std::string> str_blocks;
    std::stringstream ss(str);
    std::string buffer;
    while(std::getline(ss, buffer, sep)){
        str_blocks.push_back(buffer);
    }
    return str_blocks;
}

Board::Board()
{
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FEN2Board();
    finished = false;
}

void Board::FEN2Board()
{
    std::vector<std::string> fenBlocks = split(fen, ' ');
    assert(fenBlocks.size() == 6);

    for(int i = 0; i < fenBlocks[0].size(); i++){
        if(std::isdigit(fenBlocks[0][i])){
            fenBlocks[0].replace(i, 1, std::string(fenBlocks[0][i] - '0', '-'));
        }
    }
    std::vector<std::string> placements = split(fenBlocks[0], '/');
    assert(placements.size() == 8);
    for(int i = 0; i < Length; i++){
        for(int j = 0; j < Length; j++){
            setPieceAt(abbr2piece(placements[i][j]), i, j);
        }
    }

    activeColor = fenBlocks[1][0];
    castlability = fenBlocks[2];
    enPassantTarget = fenBlocks[3];
    halfmoveClock = std::stoi(fenBlocks[4]);
    fullmoveNumber = std::stoi(fenBlocks[5]);
}

void Board::FEN2Board(std::string fen)
{
    this->fen = fen;
    FEN2Board();
}

void Board::board2FEN()
{
    fen = "";
    int vacant;
    for(int i = 0; i < Length; i++){
        vacant = 0;
        for(int j = 0; j < Length; j++){
            if(getPieceCharAt(i, j) == '-'){
                vacant += 1;
            } else {
                if(vacant != 0){
                    fen += vacant + '0';
                    vacant = 0;
                }
                fen += getPieceCharAt(i, j);
            }
        }
        if(vacant != 0){
            fen += vacant + '0';
        }
        if(i != Length - 1){
            fen += '/';
        }
    }

    fen += " ";
    fen += activeColor;
    fen += " " + castlability;
    fen += " " + enPassantTarget;
    fen += " " + std::to_string(halfmoveClock);
    fen += " " + std::to_string(fullmoveNumber);
}

void Board::setPieceAt(Piece* piece, std::string notation)
{
    piece->placeAt(this, notation);
    piecePlacement[notation] = piece;
}

std::vector<int> Board::getIndices(std::string notation)
{
    assert(notation.size() == 2);
    assert('a' <= notation[0] && notation[0] <= 'h');
    assert('1' <= notation[1] && notation[1] <= '8');
    std::vector<int> indices;
    indices.push_back(7 - (notation[1] - '1'));
    indices.push_back(notation[0] - 'a');
    return indices;
}

std::string Board::getNotation(int i, int j)
{
    assert(0 <= i && i < Length);
    assert(0 <= j && j < Length);
    char file = 'a';
    char rank = '1';
    std::string notation;
    file += j;
    rank += Length - 1 - i;
    notation += file;
    notation += rank;
    return notation;
}

char Board::getPieceColorAt(std::string notation)
{
    return getPieceAt(notation)->color;
}

char Board::getPieceColorAt(int i, int j)
{
    return getPieceAt(i, j)->color;
}

bool Board::move(std::string origin, std::string destination)
{
    if(finished) return false;
    bool moved = getPieceAt(origin)->moveTo(destination);
    int n_moves = 0;
    if(moved){
        for(int i = 0; i < Length; i++){
            for(int j = 0; j < Length; j++){
                if(getPieceColorAt(i, j) == activeColor){
                    n_moves += getPieceAt(i, j)->getPossibleMoves().size();
                }
            }
        }
        if(n_moves == 0){
            finished = true;
            if(!inCheck(activeColor)){
                result = "0.5-0.5";
            } else {
                result = activeColor == 'w' ? "0-1" : "1-0";
            }
            std::cout << result << std::endl;
        }
    }
    return moved;
}

std::string Board::getDestination(std::string origin, int fileDirection, int rankDirection)
{
    char originFile = origin[0];
    char originRank = origin[1];
    std::string destination;
    char destinationFile = originFile + fileDirection;
    char destinationRank = originRank + rankDirection;

    if('a' <= destinationFile && destinationFile <= 'h'
    && '1' <= destinationRank && destinationRank <= '8'){
        destination += destinationFile;
        destination += destinationRank;
    } else {
        destination = "-";
    }
    return destination;
}

std::vector<std::string> Board::getAttackingSquares(char color)
{
    std::vector<std::string> squares;
    std::vector<std::string> tmp;
    for(int i = 0; i < Length; i++){
        for(int j = 0; j < Length; j++){
            if(getPieceColorAt(i, j) == color){
                tmp = getPieceAt(i, j)->getAttackingSquares();
                squares.insert(squares.end(), tmp.begin(), tmp.end());
            }
        }
    }
    return squares;
}

void Board::print()
{
    char file = 'a';
    for(int i = 0; i < Length; i++){
        std::cout << 8 - i << ' ';
        for(int j = 0; j < Length; j++){
            std::cout << getPieceCharAt(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "  ";
    for(int i = 0; i < Length; i++){
        std::cout << file << ' ';
        file += 1;
    }
    std::cout << std::endl << getFEN() << std::endl;
}

void Board::changeActiveColor()
{
    if(activeColor == 'w'){
        activeColor = 'b';
    } else {
        activeColor = 'w';
    }
}

std::string Board::getKingPosition(char color)
{
    for(int i = 0; i < Length; i++){
        for(int j = 0; j < Length; j++){
            if(getPieceAt(i, j)->name == "King"){
                if(getPieceColorAt(i, j) == color)
                    return getNotation(i, j);
            }
        }
    }
    return "-";
}

bool Board::inCheck(char color)
{
    std::string kingPosition = getKingPosition(color);
    std::vector<std::string> attacked = getAttackingSquares(color == 'w' ? 'b' : 'w');
    return std::find(attacked.begin(), attacked.end(), kingPosition) != attacked.end();
}

std::vector<std::string> Piece::removeUnableMoves(std::vector<std::string> moves)
{
    std::vector<std::string> possibleMoves;
    std::string origin = position;
    Piece* target;
    for(int i = 0; i < moves.size(); i++){
        target = board->getPieceAt(moves[i]);
        board->setPieceAt(this, moves[i]);
        board->setPieceAt(new Vacant(), origin);
        if(!board->inCheck(this->color)){
            possibleMoves.push_back(moves[i]);
        }
        board->setPieceAt(this, origin);
        board->setPieceAt(target, moves[i]);
    }
    return possibleMoves;
}

void Piece::placeAt(Board* board, std::string placement)
{
    this->board = board;
    position = placement;
}

std::string Piece::getDestination(int fileMove, int rankMove)
{
    return board->getDestination(position, fileMove, rankMove);
}

bool Piece::moveTo(std::string destination)
{
    if(this->color != board->getActiveColor()) return false;
    std::vector<std::string> moves = getPossibleMoves();
    if(std::find(moves.begin(), moves.end(), destination) == moves.end()) return false;

    bool captured = (board->getPieceCharAt(destination) != '-');
    std::string origin = position;
    board->setPieceAt(this, destination);
    board->setPieceAt(new Vacant(), origin);

    board->changeActiveColor();
    board->enPassantTarget = "-";
    if(captured){
        board->halfmoveClock = 0;
    } else {
        board->halfmoveClock += 1;
    }
    if(this->color == 'b') board->fullmoveNumber += 1;
    board->board2FEN();
    return true;
}

std::vector<std::string> Pawn::getPossibleMoves()
{
    std::vector<std::string> moves;
    std::string candidate;
    char homeRank = color == 'w' ? '2' : '7';

    candidate = getDestination(0, movingDirection);
    if(candidate != "-"){
        if(board->getPieceCharAt(candidate) == '-'){
            moves.push_back(candidate);
            if(position[1] == homeRank){
                candidate = getDestination(0, 2 * movingDirection);
                if(board->getPieceCharAt(candidate) == '-'){
                    moves.push_back(candidate);
                }
            }
        }
    }

    for(int i = 0; i < 2; i++){
        candidate = getDestination(2 * i - 1, movingDirection);
        if(candidate != "-"){
            if(board->getPieceColorAt(candidate) == opponentColor){
                moves.push_back(candidate);
            } else if(candidate == board->enPassantTarget){
                moves.push_back(candidate);
            }
        }
    }

    return removeUnableMoves(moves);
}

std::vector<std::string> Pawn::getAttackingSquares()
{
    std::string candidate;
    std::vector<std::string> squares;
    for(int i = 0; i < 2; i++){
        candidate = getDestination(2 * i - 1, movingDirection);
        if(candidate != "-"){
            squares.push_back(candidate);
        }
    }
    return squares;
}

bool Pawn::moveTo(std::string destination)
{
    if(this->color != board->getActiveColor()) return false;
    std::vector<std::string> moves = getPossibleMoves();
    if(std::find(moves.begin(), moves.end(), destination) == moves.end()) return false;

    std::string origin = position;
    if(std::abs(origin[1] - destination[1]) > 1){
        board->enPassantTarget = origin[0];
        board->enPassantTarget += (origin[1] + destination[1]) / 2;
    } else if(destination == board->enPassantTarget){
        board->setPieceAt(
            new Vacant(),
            board->getDestination(board->enPassantTarget, 0, -movingDirection)
        );
        board->enPassantTarget = "-";
    } else {
        board->enPassantTarget = "-";
    }
    board->setPieceAt(this, destination);
    board->setPieceAt(new Vacant(), origin);

    board->changeActiveColor();
    board->halfmoveClock = 0;
    if(this->color == 'b') board->fullmoveNumber += 1;
    board->board2FEN();
    return true;
}

std::vector<std::string> Knight::getPossibleMoves()
{
    return removeUnableMoves(getAttackingSquares());
}

std::vector<std::string> Knight::getAttackingSquares()
{
    std::vector<std::string> squares;
    std::string candidate;
    static const int direction[8][2] = {
        {-1, 2}, {1, 2},  // front (from white's perspective)
        {2, 1}, {2, -1},  // right
        {-1, -2}, {1, -2},  // back
        {-2, 1}, {-2, -1}  // left
    };

    for(int i = 0; i < 8; i++){
        candidate = getDestination(direction[i][0], direction[i][1]);
        if(candidate == "-") continue;
        if(board->getPieceColorAt(candidate) != this->color)
            squares.push_back(candidate);
    }

    return squares;
}

std::vector<std::string> Bishop::getPossibleMoves()
{
    return removeUnableMoves(getAttackingSquares());
}

std::vector<std::string> Bishop::getAttackingSquares()
{
    std::vector<std::string> squares;
    std::string candidate;
    static const int direction[4][2] = {
        {-1, 1}, {1, 1},
        {-1, -1}, {1, -1}
    };
    char targetColor;

    for(int i = 0; i < 4; i++){
        candidate = position;
        while(true){
            candidate = board->getDestination(candidate, direction[i][0], direction[i][1]);
            if(candidate == "-") break;
            targetColor = board->getPieceColorAt(candidate);
            if(targetColor == this->color){
                break;
            } else if(targetColor == opponentColor){
                squares.push_back(candidate);
                break;
            } else {
                squares.push_back(candidate);
            }
        }
    }

    return squares;
}

std::vector<std::string> Rook::getPossibleMoves()
{
    return removeUnableMoves(getAttackingSquares());
}

std::vector<std::string> Rook::getAttackingSquares()
{
    std::vector<std::string> squares;
    std::string candidate;
    static const int direction[4][2] = {
            {0, 1},
        {-1, 0}, {1, 0},
            {0, -1}
    };
    char targetColor;

    for(int i = 0; i < 4; i++){
        candidate = position;
        while(true){
            candidate = board->getDestination(candidate, direction[i][0], direction[i][1]);
            if(candidate == "-") break;
            targetColor = board->getPieceColorAt(candidate);
            if(targetColor == this->color){
                break;
            } else if(targetColor == opponentColor){
                squares.push_back(candidate);
                break;
            } else {
                squares.push_back(candidate);
            }
        }
    }

    return squares;
}

bool Rook::moveTo(std::string destination)
{
    if(this->color != board->getActiveColor()) return false;
    std::vector<std::string> moves = getPossibleMoves();
    if(std::find(moves.begin(), moves.end(), destination) == moves.end()) return false;

    bool captured = (board->getPieceCharAt(destination) != '-');
    std::string origin = position;
    int pos;
    if(position == "a1"){
        pos = board->castlability.find('Q');
        if(pos != -1) board->castlability.erase(pos, 1);
    } else if(position == "h1"){
        pos = board->castlability.find('K');
        if(pos != -1) board->castlability.erase(pos, 1);
    } else if(position == "a8"){
        pos = board->castlability.find('q');
        if(pos != -1) board->castlability.erase(pos, 1);
    } else if(position == "h8"){
        pos = board->castlability.find('k');
        if(pos != -1) board->castlability.erase(pos, 1);
    }
    if(board->castlability.empty()) board->castlability = "-";

    board->setPieceAt(this, destination);
    board->setPieceAt(new Vacant(), origin);

    board->changeActiveColor();
    board->enPassantTarget = "-";
    if(captured){
        board->halfmoveClock = 0;
    } else {
        board->halfmoveClock += 1;
    }
    if(this->color == 'b') board->fullmoveNumber += 1;
    board->board2FEN();
    return true;
}

std::vector<std::string> Queen::getPossibleMoves()
{
    return removeUnableMoves(getAttackingSquares());
}

std::vector<std::string> Queen::getAttackingSquares()
{
    std::vector<std::string> squares;
    std::string candidate;
    static const int direction[8][2] = {
        {-1, 1}, {0, 1}, {1, 1},
        {-1, 0},         {1, 0},
        {-1, -1}, {0, -1}, {1, -1}
    };
    char targetColor;

    for(int i = 0; i < 8; i++){
        candidate = position;
        while(true){
            candidate = board->getDestination(candidate, direction[i][0], direction[i][1]);
            if(candidate == "-") break;
            targetColor = board->getPieceColorAt(candidate);
            if(targetColor == this->color){
                break;
            } else if(targetColor == opponentColor){
                squares.push_back(candidate);
                break;
            } else {
                squares.push_back(candidate);
            }
        }
    }

    return squares;
}

std::vector<std::string> King::getPossibleMoves()
{
    std::vector<std::string> moves = getAttackingSquares();
    std::vector<std::string> attacked = board->getAttackingSquares(opponentColor);

    if(board->castlability != "-"){
        if(color == 'w' && std::find(attacked.begin(), attacked.end(), "e1") == attacked.end()){
            if(board->castlability.find('K') != -1 && board->getPieceCharAt("f1") == '-' && board->getPieceCharAt("g1") == '-' && std::find(attacked.begin(), attacked.end(), "f1") == attacked.end() && std::find(attacked.begin(), attacked.end(), "g1") == attacked.end()){
                moves.push_back("g1");
            }
            if(board->castlability.find('Q') != -1 && board->getPieceCharAt("b1") == '-' && board->getPieceCharAt("c1") == '-' && board->getPieceCharAt("d1") == '-' && std::find(attacked.begin(), attacked.end(), "c1") == attacked.end() && std::find(attacked.begin(), attacked.end(), "d1") == attacked.end()){
                moves.push_back("c1");
            }
        } else if(color == 'b' && std::find(attacked.begin(), attacked.end(), "e8") == attacked.end()){
            if(board->castlability.find('k') != -1 && board->getPieceCharAt("f8") == '-' && board->getPieceCharAt("g8") == '-' && std::find(attacked.begin(), attacked.end(), "f8") == attacked.end() && std::find(attacked.begin(), attacked.end(), "g8") == attacked.end()){
                moves.push_back("g8");
            }
            if(board->castlability.find('q') != -1 && board->getPieceCharAt("b8") == '-' && board->getPieceCharAt("c8") == '-' && board->getPieceCharAt("d8") == '-' && std::find(attacked.begin(), attacked.end(), "c8") == attacked.end() && std::find(attacked.begin(), attacked.end(), "d8") == attacked.end()){
                moves.push_back("c8");
            }
        }
    }

    return removeUnableMoves(moves);
}

std::vector<std::string> King::getAttackingSquares()
{
    std::vector<std::string> squares;
    std::string candidate;
    static const int direction[8][2] = {
        {-1, 1}, {0, 1}, {1, 1},
        {-1, 0},         {1, 0},
        {-1, -1}, {0, -1}, {1, -1}
    };
    char targetColor;

    for(int i = 0; i < 8; i++){
        candidate = getDestination(direction[i][0], direction[i][1]);
        if(candidate == "-") continue;
        targetColor = board->getPieceColorAt(candidate);
        if(targetColor == this->color){
            continue;
        } else {
            squares.push_back(candidate);
        }
    }

    return squares;
}

bool King::moveTo(std::string destination)
{
    if(this->color != board->getActiveColor()) return false;
    std::vector<std::string> moves = getPossibleMoves();
    if(std::find(moves.begin(), moves.end(), destination) == moves.end()) return false;

    bool captured = (board->getPieceCharAt(destination) != '-');
    std::string origin = position;
    board->setPieceAt(this, destination);
    board->setPieceAt(new Vacant(), origin);

    board->changeActiveColor();
    int pos;
    if(this->color == 'w'){
        pos = board->castlability.find('K');
        if(pos != -1){
            if(destination == "g1"){
                board->setPieceAt(board->getPieceAt("h1"), "f1");
                board->setPieceAt(new Vacant(), "h1");
            }
            board->castlability.erase(pos, 1);
        }
        pos = board->castlability.find('Q');
        if(pos != -1){
            if(destination == "c1"){
                board->setPieceAt(board->getPieceAt("a1"), "d1");
                board->setPieceAt(new Vacant(), "a1");
            }
            board->castlability.erase(pos, 1);
        }
    } else {
        pos = board->castlability.find('k');
        if(pos != -1){
            if(destination == "g8"){
                board->setPieceAt(board->getPieceAt("h8"), "f8");
                board->setPieceAt(new Vacant(), "h8");
            }
            board->castlability.erase(pos, 1);
        }
        pos = board->castlability.find('q');
        if(pos != -1){
            if(destination == "c8"){
                board->setPieceAt(board->getPieceAt("a8"), "d8");
                board->setPieceAt(new Vacant(), "a8");
            }
            board->castlability.erase(pos, 1);
        }
    }
    board->enPassantTarget = "-";
    if(captured){
        board->halfmoveClock = 0;
    } else {
        board->halfmoveClock += 1;
    }
    if(this->color == 'b') board->fullmoveNumber += 1;
    board->board2FEN();
    return true;
}

std::vector<std::string> Vacant::getPossibleMoves()
{
    std::vector<std::string> moves;
    return moves;
}

// int main()
// {
//     Board board = Board();
//     board.print();
//     board.move("e2", "e4");
//     board.move("e7", "e5");
//     board.move("d1", "h5");
//     board.print();
//     std::vector<std::string> moves = board.getPieceAt("f7")->getPossibleMoves();
//     for(int i = 0; i < moves.size(); i++){
//         std::cout << moves[i] << std::endl;
//     }
// }
