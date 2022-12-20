#ifndef PIECE_H
#define PIECE_H

#include <locale>
#include <string>
#include <vector>
#include <wx/stdpaths.h>

class Board;

class Piece
{
public:
    Piece(char color, std::string name, char abbr);
    void placeAt(Board* board, std::string placement);
    std::string getDestination(int fileMove, int RankMove);
    virtual std::vector<std::string> getPossibleMoves() = 0;
    virtual std::vector<std::string> getAttackingSquares() = 0;
    virtual bool moveTo(std::string destination);
    std::vector<std::string> removeUnableMoves(std::vector<std::string> moves);
    char const color;
    char const opponentColor;
    std::string const name;
    char const abbreviation;

protected:
    Board* board;
    std::string position;
};

class Pawn : public Piece
{
public:
    Pawn(char color) : Piece(color, "Pawn", (color == 'w' ? 'P' : 'p')), movingDirection(color == 'w' ? 1 : -1){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares();
    bool moveTo(std::string destination);

protected:
    int const movingDirection;
};

class Knight : public Piece
{
public:
    Knight(char color) : Piece(color, "Knight", (color == 'w' ? 'N' : 'n')){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares();
};

class Bishop : public Piece
{
public:
    Bishop(char color) : Piece(color, "Bishop", (color == 'w' ? 'B' : 'b')){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares();
};

class Rook : public Piece
{
public:
    Rook(char color) : Piece(color, "Rook", (color == 'w' ? 'R' : 'r')){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares();
    bool moveTo(std::string destination);
};

class Queen : public Piece
{
public:
    Queen(char color) : Piece(color, "Queen", (color == 'w' ? 'Q' : 'q')){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares();
};

class King : public Piece
{
public:
    King(char color) : Piece(color, "King", (color == 'w' ? 'K' : 'k')){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares();
    bool moveTo(std::string destination);
};

class Vacant : public Piece
{
public:
    Vacant() : Piece('-', "Vacant", '-'){}
    std::vector<std::string> getPossibleMoves();
    std::vector<std::string> getAttackingSquares() { return getPossibleMoves(); }
    bool moveTo(std::string destination) { return false; }
};

Piece* abbr2piece(char abbreviation);

#endif
