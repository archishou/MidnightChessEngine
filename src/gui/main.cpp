#include "main.h"
#include "frame.h"

IMPLEMENT_APP(ChessApp)

bool ChessApp::OnInit()
{
    wxInitAllImageHandlers();
    Frame *frame = new Frame(wxT("Chess"));
    frame->Show(true);

    return true;
}
