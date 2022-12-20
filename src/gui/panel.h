#include <map>
#include <string>
#include <vector>
#include <wx/wx.h>
#include "symbol.h"
#include "board.h"

class Panel : public wxPanel
{
public:
    Panel(wxFrame *parent, Board *chessboard);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMove(wxMouseEvent& event);

private:
    int SquareLength();
    void DrawSquare(wxPaintDC &dc, int x, int y);
    void LoadPiece();
    wxStatusBar *statusbar;
    Board *board;
    int boardLength;
    std::vector<Symbol*> symbol;
    std::string origin;
    std::map<char, wxString> symbolPath;
};
