#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame {
private:
    static const int BOARD_SIZE = 3;
    wxButton* board[BOARD_SIZE][BOARD_SIZE];
    wxButton* resetButton;
    wxStaticText* xScoreLabel;
    wxStaticText* oScoreLabel;
    int xScore;
    int oScore;
    bool isXTurn;
    bool gameOver;
    wxStaticText* turnLabel;

    void OnButtonClick(wxCommandEvent& evt);
    void CheckWinner();
    void ResetGame(wxCommandEvent& evt);
    void OnButtonHover(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    bool isPVPMode;
    void AIMove();  // New function for AI moves
    int Minimax(bool isMaximizing, int depth, int alpha, int beta);
    bool IsMovesLeft();
    int EvaluateBoard();
    std::pair<int, int> GetBestMove();
    wxString GetBoardState(int row, int col);

public:
    MainFrame(const wxString& title);

    wxDECLARE_EVENT_TABLE();
};

#endif //MAINFRAME_H