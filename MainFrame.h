// MainFrame.h
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame {
private:
    static const int ROWS = 6;
    static const int COLS = 7;
    wxButton* board[ROWS][COLS];
    wxButton* resetButton;
    wxStaticText* playerScoreLabel;
    wxStaticText* aiScoreLabel;
    int playerScore;
    int aiScore;
    bool gameOver;
    wxStaticText* statusLabel;

    void OnButtonClick(wxCommandEvent& evt);
    void CheckWinner();
    void ResetGame(wxCommandEvent& evt);
    void OnButtonHover(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    void AIMove();
    int Minimax(bool isMaximizing, int depth, int alpha, int beta);
    bool IsMovesLeft();

    void UpdateScore(const wxString &winner);

    int EvaluateBoard();
    int GetLowestEmptyRow(int col);
    std::pair<int, int> GetBestMove();
    wxString GetBoardState(int row, int col);
    int EvaluateWindow(wxString window[4]);
    bool IsValidMove(int col);

public:
    MainFrame(const wxString& title);

    wxDECLARE_EVENT_TABLE();
};

#endif //MAINFRAME_H