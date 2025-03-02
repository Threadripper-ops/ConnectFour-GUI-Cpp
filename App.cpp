#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    MainFrame* frame = new MainFrame("Tic Tac Toe");
    frame->Show(true);
    return true;
}