#include "symbol.h"

Symbol::Symbol(const wxString path)
{
    captured = false;
    dragging = false;
    img = wxImage(path, wxBITMAP_TYPE_ANY);
}

void Symbol::Draw(wxDC& dc, int length)
{
    spacingLength = length;
    symbolSize = length * 0.8;
    spacingOffset = length * 0.1;
    if(dragging){
        dc.DrawBitmap(
            wxBitmap(img.Scale(symbolSize, symbolSize)),
            pixelX, pixelY, false
        );
    } else if(!captured){
        dc.DrawBitmap(
            wxBitmap(img.Scale(symbolSize, symbolSize)),
            boardX * length + spacingOffset, boardY * length + spacingOffset, false
        );
    }
}

bool Symbol::BeginMove(wxPoint pt)
{
    if(captured){
        return false;
    }
    pixelX = boardX * spacingLength + spacingOffset;
    pixelY = boardY * spacingLength + spacingOffset;
    if(pixelX <= pt.x && pt.x <= pixelX + symbolSize && pixelY <= pt.y && pt.y <= pixelY + symbolSize){
        pixelX = pt.x - symbolSize / 2;
        pixelY = pt.y - symbolSize / 2;
        dragging = true;
        return true;
    } else {
        return false;
    }
}

void Symbol::FinishMove(wxPoint pt, bool moved)
{
    if(dragging){
        if(moved){
            boardX = pt.x / spacingLength;
            boardY = pt.y / spacingLength;
        }
        dragging = false;
    }
}

void Symbol::Move(wxPoint pt)
{
    if(dragging){
        pixelX = pt.x - symbolSize / 2;
        pixelY = pt.y - symbolSize / 2;
    }
}
