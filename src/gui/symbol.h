#include <wx/wx.h>

class Symbol
{
public:
    Symbol(const wxString path);
    void PlaceAt(int x, int y) { boardX = x; boardY = y; }
    void Draw(wxDC& dc, int length);
    bool BeginMove(wxPoint pt);
    void FinishMove(wxPoint pt, bool moved);
    void Move(wxPoint pt);
    bool isDragging() { return dragging; }
    int getBoardX() { return boardX; }
    int getBoardY() { return boardY; }
    void remove() { captured = true; }

private:
    int spacingLength;
    int symbolSize;
    int spacingOffset;
    bool dragging;
    bool captured;
    int boardX, boardY;
    int pixelX, pixelY;
    wxImage img;
};