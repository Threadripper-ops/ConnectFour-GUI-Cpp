// MainFrame.cpp
#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(30, 30), wxSize(1000, 800)) {
    gameOver = false;
    playerScore = 0;
    aiScore = 0;

    this->SetBackgroundColour(wxColor(44, 44, 84));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title
    wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "Connect Four",
                                              wxDefaultPosition, wxDefaultSize,
                                              wxALIGN_CENTER_HORIZONTAL);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(24);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColor(255, 255, 255));

    // Status panel
    wxPanel* statusPanel = new wxPanel(this, wxID_ANY);
    statusPanel->SetBackgroundColour(wxColor(44, 44, 84));
    wxBoxSizer* statusSizer = new wxBoxSizer(wxHORIZONTAL);
    statusLabel = new wxStaticText(statusPanel, wxID_ANY, "Your Turn (Yellow)");
    statusLabel->SetFont(titleFont.Scale(0.7));
    statusLabel->SetForegroundColour(wxColor(255, 255, 255));
    statusSizer->Add(statusLabel, 0, wxALL | wxALIGN_CENTER, 10);
    statusPanel->SetSizer(statusSizer);

    // Game board
    wxPanel* gamePanel = new wxPanel(this, wxID_ANY);
    gamePanel->SetBackgroundColour(wxColor(44, 44, 84));
    wxGridSizer* grid = new wxGridSizer(ROWS, COLS, 5, 5);

    wxFont buttonFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLS; ++j) {
            board[i][j] = new wxButton(gamePanel, 10000 + (i * COLS + j), "",
                                     wxDefaultPosition, wxSize(80, 80));
            board[i][j]->SetFont(buttonFont);
            board[i][j]->SetBackgroundColour(wxColor(240, 240, 255));
            board[i][j]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
            board[i][j]->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnButtonHover, this);
            board[i][j]->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnButtonLeave, this);
            grid->Add(board[i][j], 1, wxEXPAND);
        }
    }
    gamePanel->SetSizer(grid);

    // Score panel
    wxPanel* scorePanel = new wxPanel(this, wxID_ANY);
    scorePanel->SetBackgroundColour(wxColor(44, 44, 84));
    wxBoxSizer* scoreSizer = new wxBoxSizer(wxHORIZONTAL);

    wxFont scoreFont = titleFont.Scale(0.8);
    playerScoreLabel = new wxStaticText(scorePanel, wxID_ANY, "Player: 0");
    aiScoreLabel = new wxStaticText(scorePanel, wxID_ANY, "AI: 0");
    playerScoreLabel->SetFont(scoreFont);
    aiScoreLabel->SetFont(scoreFont);
    playerScoreLabel->SetForegroundColour(wxColor(255, 255, 0));
    aiScoreLabel->SetForegroundColour(wxColor(255, 0, 0));

    scoreSizer->Add(playerScoreLabel, 0, wxALL | wxALIGN_CENTER, 15);
    scoreSizer->AddSpacer(50);
    scoreSizer->Add(aiScoreLabel, 0, wxALL | wxALIGN_CENTER, 15);
    scorePanel->SetSizer(scoreSizer);

    // Reset button
    resetButton = new wxButton(this, wxID_ANY, "New Game",
                             wxDefaultPosition, wxSize(150, 50));
    resetButton->SetFont(scoreFont);
    resetButton->SetBackgroundColour(wxColor(65, 105, 225));
    resetButton->SetForegroundColour(wxColor(255, 255, 255));
    resetButton->Bind(wxEVT_BUTTON, &MainFrame::ResetGame, this);

    mainSizer->AddSpacer(20);
    mainSizer->Add(titleText, 0, wxALIGN_CENTER | wxALL, 20);
    mainSizer->Add(statusPanel, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(gamePanel, 0, wxALIGN_CENTER | wxALL, 20);
    mainSizer->Add(scorePanel, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(resetButton, 0, wxALIGN_CENTER | wxALL, 15);
    mainSizer->AddSpacer(20);

    this->SetSizer(mainSizer);
}

void MainFrame::OnButtonClick(wxCommandEvent& evt) {
    if(gameOver) return;

    int buttonId = evt.GetId() - 10000;
    int clickedCol = buttonId % COLS;
    int lowestEmptyRow = GetLowestEmptyRow(clickedCol);

    if(lowestEmptyRow < 0) {
        wxBell();  // Provide feedback for invalid move
        return;
    }

    // Player move
    board[lowestEmptyRow][clickedCol]->SetLabel("P");
    board[lowestEmptyRow][clickedCol]->SetBackgroundColour(wxColor(255, 255, 0));
    board[lowestEmptyRow][clickedCol]->Refresh();

    CheckWinner();
    if(!gameOver) {
        statusLabel->SetLabel("AI's Turn (Red)");
        statusLabel->Refresh();
        AIMove();
        if(!gameOver) {
            statusLabel->SetLabel("Your Turn (Yellow)");
            statusLabel->Refresh();
        }
    }
}

void MainFrame::OnButtonHover(wxMouseEvent& evt) {
    if(gameOver) {
        evt.Skip();
        return;
    }

    wxButton* button = wxDynamicCast(evt.GetEventObject(), wxButton);
    if(!button || button == resetButton) {
        evt.Skip();
        return;
    }

    int buttonId = button->GetId() - 10000;
    int col = buttonId % COLS;
    int lowestEmptyRow = GetLowestEmptyRow(col);

    if(lowestEmptyRow >= 0) {
        wxColor hoverColor = wxColor(255, 255, 200);  // Light yellow for hover
        board[lowestEmptyRow][col]->SetBackgroundColour(hoverColor);
        board[lowestEmptyRow][col]->Refresh();
    }

    evt.Skip();
}

void MainFrame::OnButtonLeave(wxMouseEvent& evt) {
    wxButton* button = wxDynamicCast(evt.GetEventObject(), wxButton);
    if(!button || button == resetButton) {
        evt.Skip();
        return;
    }

    int buttonId = button->GetId() - 10000;
    int col = buttonId % COLS;
    int lowestEmptyRow = GetLowestEmptyRow(col);

    if(lowestEmptyRow >= 0) {
        board[lowestEmptyRow][col]->SetBackgroundColour(wxColor(240, 240, 255));
        board[lowestEmptyRow][col]->Refresh();
    }

    evt.Skip();
}

int MainFrame::GetLowestEmptyRow(int col) {
    for(int row = ROWS-1; row >= 0; row--) {
        if(board[row][col]->GetLabel().empty()) {
            return row;
        }
    }
    return -1;
}

int MainFrame::EvaluateWindow(wxString window[4]) {
    int playerCount = 0;
    int aiCount = 0;

    for(int i = 0; i < 4; i++) {
        if(window[i] == "P") playerCount++;
        else if(window[i] == "A") aiCount++;
    }

    if(aiCount == 4) return 4;      // AI wins
    if(playerCount == 4) return -4;  // Player wins

    if(aiCount == 3 && playerCount == 0) return 3;
    if(playerCount == 3 && aiCount == 0) return -3;
    if(aiCount == 2 && playerCount == 0) return 2;
    if(playerCount == 2 && aiCount == 0) return -2;
    if(aiCount == 1 && playerCount == 0) return 1;
    if(playerCount == 1 && aiCount == 0) return -1;

    return 0;  // No advantage
}

wxString MainFrame::GetBoardState(int row, int col) {
    return board[row][col]->GetLabel();
}

void MainFrame::CheckWinner() {
    // Horizontal check
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col <= COLS-4; col++) {
            wxString window[4];
            for(int k = 0; k < 4; k++) {
                window[k] = GetBoardState(row, col+k);
            }
            int score = EvaluateWindow(window);
            if(score == 4) {
                gameOver = true;
                UpdateScore("AI");
                return;
            } else if(score == -4) {
                gameOver = true;
                UpdateScore("Player");
                return;
            }
        }
    }

    // Vertical check
    for(int row = 0; row <= ROWS-4; row++) {
        for(int col = 0; col < COLS; col++) {
            wxString window[4];
            for(int k = 0; k < 4; k++) {
                window[k] = GetBoardState(row+k, col);
            }
            int score = EvaluateWindow(window);
            if(score == 4) {
                gameOver = true;
                UpdateScore("AI");
                return;
            } else if(score == -4) {
                gameOver = true;
                UpdateScore("Player");
                return;
            }
        }
    }

    // Diagonal checks
    for(int row = 0; row <= ROWS-4; row++) {
        for(int col = 0; col <= COLS-4; col++) {
            wxString window1[4], window2[4];
            for(int k = 0; k < 4; k++) {
                window1[k] = GetBoardState(row+k, col+k);
                window2[k] = GetBoardState(row+k, col+3-k);
            }
            int score = EvaluateWindow(window1);
            if(score == 4) {
                gameOver = true;
                UpdateScore("AI");
                return;
            } else if(score == -4) {
                gameOver = true;
                UpdateScore("Player");
                return;
            }
        }
    }

    // Check for draw
    bool isDraw = true;
    for(int col = 0; col < COLS; col++) {
        if(GetLowestEmptyRow(col) >= 0) {
            isDraw = false;
            break;
        }
    }

    if(isDraw) {
        gameOver = true;
        statusLabel->SetLabel("Game Over - Draw!");
        wxMessageBox("It's a Draw!", "Game Over", wxOK | wxICON_INFORMATION);
    }
}

void MainFrame::ResetGame(wxCommandEvent& evt) {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLS; ++j) {
            board[i][j]->SetLabel("");
            board[i][j]->SetBackgroundColour(wxColor(240, 240, 255));
            board[i][j]->Refresh();
        }
    }

    gameOver = false;
    statusLabel->SetLabel("Your Turn (Yellow)");
    statusLabel->Refresh();
}

void MainFrame::AIMove() {
    if(gameOver) return;

    wxMilliSleep(500);  // Add small delay for better user experience

    std::pair<int, int> bestMove = GetBestMove();
    int row = bestMove.first;
    int col = bestMove.second;

    board[row][col]->SetLabel("A");
    board[row][col]->SetBackgroundColour(wxColor(255, 0, 0));
    board[row][col]->Refresh();

    CheckWinner();
}

std::pair<int, int> MainFrame::GetBestMove() {
    int bestVal = -1000;
    std::pair<int, int> bestMove = {-1, -1};

    for(int col = 0; col < COLS; col++) {
        int row = GetLowestEmptyRow(col);
        if(row >= 0) {
            board[row][col]->SetLabel("A");
            int moveVal = Minimax(false, 5, -1000, 1000);  // Depth 5 for better performance
            board[row][col]->SetLabel("");

            if(moveVal > bestVal) {
                bestMove = {row, col};
                bestVal = moveVal;
            }
        }
    }

    return bestMove;
}

int MainFrame::Minimax(bool isMaximizing, int depth, int alpha, int beta) {
    if(depth == 0) return EvaluateBoard();

    int score = EvaluateBoard();
    if(score == 100 || score == -100) return score;
    if(!IsMovesLeft()) return 0;

    if(isMaximizing) {
        int maxEval = -1000;
        for(int col = 0; col < COLS; col++) {
            int row = GetLowestEmptyRow(col);
            if(row >= 0) {
                board[row][col]->SetLabel("A");
                int eval = Minimax(false, depth - 1, alpha, beta);
                board[row][col]->SetLabel("");
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if(beta <= alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = 1000;
        for(int col = 0; col < COLS; col++) {
            int row = GetLowestEmptyRow(col);
            if(row >= 0) {
                board[row][col]->SetLabel("P");
                int eval = Minimax(true, depth - 1, alpha, beta);
                board[row][col]->SetLabel("");
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if(beta <= alpha) break;
            }
        }
        return minEval;
    }
}

int MainFrame::EvaluateBoard() {
    int score = 0;

    // Horizontal evaluation
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col <= COLS-4; col++) {
            wxString window[4];
            for(int k = 0; k < 4; k++) {
                window[k] = GetBoardState(row, col+k);
            }
            score += EvaluateWindow(window);
        }
    }

    // Vertical evaluation
    for(int row = 0; row <= ROWS-4; row++) {
        for(int col = 0; col < COLS; col++) {
            wxString window[4];
            for(int k = 0; k < 4; k++) {
                window[k] = GetBoardState(row+k, col);
            }
            score += EvaluateWindow(window);
        }
    }

    // Diagonal evaluations (both directions)
    for(int row = 0; row <= ROWS-4; row++) {
        for(int col = 0; col <= COLS-4; col++) {
            wxString window1[4], window2[4];
            for(int k = 0; k < 4; k++) {
                window1[k] = GetBoardState(row+k, col+k);
                window2[k] = GetBoardState(row+k, col+3-k);
            }
            score += EvaluateWindow(window1);
            score += EvaluateWindow(window2);
        }
    }

    return score;
}

bool MainFrame::IsMovesLeft() {
    for(int col = 0; col < COLS; col++) {
        if(GetLowestEmptyRow(col) >= 0) return true;
    }
    return false;
}

void MainFrame::UpdateScore(const wxString& winner) {
    if(winner == "Player") {
        playerScore++;
        playerScoreLabel->SetLabel(wxString::Format("Player: %d", playerScore));
        statusLabel->SetLabel("Game Over - Player Wins!");
    } else {
        aiScore++;
        aiScoreLabel->SetLabel(wxString::Format("AI: %d", aiScore));
        statusLabel->SetLabel("Game Over - AI Wins!");
    }

    wxMessageBox(winner + " Wins!", "Game Over", wxOK | wxICON_INFORMATION);
}