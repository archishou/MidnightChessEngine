#include "piece.h"

Piece::Piece(char color, std::string name, char abbr) : color(color), opponentColor(color == 'w' ? 'b' : 'w'), name(name), abbreviation(abbr)
{
}

Piece* abbr2piece(char abbreviation)
{
    char abbr;
    char color;
    if('A' < abbreviation && abbreviation < 'Z'){
        abbr = std::tolower(abbreviation, std::locale());
        color = 'w';
    } else if('a' < abbreviation && abbreviation < 'z'){
        abbr = abbreviation;
        color = 'b';
    }
    Piece* piece;
    switch(abbr){
        case 'p':
            piece = new Pawn(color);
            break;
        case 'n':
            piece = new Knight(color);
            break;
        case 'b':
            piece = new Bishop(color);
            break;
        case 'r':
            piece = new Rook(color);
            break;
        case 'q':
            piece = new Queen(color);
            break;
        case 'k':
            piece = new King(color);
            break;
        default:
            piece = new Vacant();
            break;
    }
    return piece;
}
