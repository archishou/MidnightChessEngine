#include "frame.h"
#include "panel.h"
#include "board.h"

Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(480, 520))
{
    wxStatusBar *sb = CreateStatusBar();
    Board *board = new Board();
    Panel *panel = new Panel(this, board);
    panel->SetFocus();
}