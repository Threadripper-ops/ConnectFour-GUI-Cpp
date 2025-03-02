#include "MainFrame.h"

#include "GameModeDialog.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxPoint(30, 30), wxSize(900, 900)) {
    isXTurn = true;
    gameOver = false;
    xScore = 0;
    oScore = 0;

    // Set background color
    this->SetBackgroundColour(wxColor(44, 44, 84));  // Dark blue background

    // Create main vertical sizer with margins
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create title text
    wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "Tic Tac Toe",
                                              wxDefaultPosition, wxDefaultSize,
                                              wxALIGN_CENTER_HORIZONTAL);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(24);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColor(255, 255, 255));  // White text

    // Status panel
    wxPanel* statusPanel = new wxPanel(this, wxID_ANY);
    statusPanel->SetBackgroundColour(wxColor(44, 44, 84));  // Match frame background
    wxBoxSizer* statusSizer = new wxBoxSizer(wxHORIZONTAL);
    turnLabel = new wxStaticText(statusPanel, wxID_ANY, "Current Turn: X");
    turnLabel->SetFont(titleFont.Scale(0.7));
    turnLabel->SetForegroundColour(wxColor(255, 255, 255));  // White text
    statusSizer->Add(turnLabel, 0, wxALL | wxALIGN_CENTER, 10);
    statusPanel->SetSizer(statusSizer);

    // Game board
    wxPanel* gamePanel = new wxPanel(this, wxID_ANY);
    gamePanel->SetBackgroundColour(wxColor(44, 44, 84));  // Match frame background
    wxGridSizer* grid = new wxGridSizer(3, 3, 10, 10);  // Increased gap between buttons

    // Create and style the game buttons
    wxFont buttonFont(28, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = new wxButton(gamePanel, 10000 + (i * BOARD_SIZE + j), "",
                                     wxDefaultPosition, wxSize(100, 100));
            board[i][j]->SetFont(buttonFont);
            board[i][j]->SetBackgroundColour(wxColor(240, 240, 255));  // Lighter background
            board[i][j]->SetForegroundColour(wxColor(50, 50, 50));     // Dark text
            board[i][j]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
            board[i][j]->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnButtonHover, this);
            board[i][j]->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnButtonLeave, this);
            grid->Add(board[i][j], 1, wxEXPAND | wxALL, 5);
        }
    }
    gamePanel->SetSizer(grid);

    // Score panel
    wxPanel* scorePanel = new wxPanel(this, wxID_ANY);
    scorePanel->SetBackgroundColour(wxColor(44, 44, 84));  // Match frame background
    wxBoxSizer* scoreSizer = new wxBoxSizer(wxHORIZONTAL);

    wxFont scoreFont = titleFont.Scale(0.8);
    xScoreLabel = new wxStaticText(scorePanel, wxID_ANY, "X Score: 0");
    oScoreLabel = new wxStaticText(scorePanel, wxID_ANY, "O Score: 0");
    xScoreLabel->SetFont(scoreFont);
    oScoreLabel->SetFont(scoreFont);
    xScoreLabel->SetForegroundColour(wxColor(255, 255, 255));  // White text
    oScoreLabel->SetForegroundColour(wxColor(255, 255, 255));  // White text

    scoreSizer->Add(xScoreLabel, 0, wxALL | wxALIGN_CENTER, 15);
    scoreSizer->AddSpacer(50);  // Increased spacing
    scoreSizer->Add(oScoreLabel, 0, wxALL | wxALIGN_CENTER, 15);
    scorePanel->SetSizer(scoreSizer);

    // Reset button
    resetButton = new wxButton(this, wxID_ANY, "New Game",
                             wxDefaultPosition, wxSize(150, 50));
    resetButton->SetFont(scoreFont);
    resetButton->SetBackgroundColour(wxColor(65, 105, 225));  // Royal blue
    resetButton->SetForegroundColour(wxColor(255, 255, 255));  // White text
    resetButton->Bind(wxEVT_BUTTON, &MainFrame::ResetGame, this);
    resetButton->Bind(wxEVT_ENTER_WINDOW, &MainFrame::OnButtonHover, this);
    resetButton->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::OnButtonLeave, this);

    // Add all elements to main sizer
    mainSizer->AddSpacer(20);
    mainSizer->Add(titleText, 0, wxALIGN_CENTER | wxALL, 20);
    mainSizer->Add(statusPanel, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(gamePanel, 0, wxALIGN_CENTER | wxALL, 20);
    mainSizer->Add(scorePanel, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(resetButton, 0, wxALIGN_CENTER | wxALL, 15);
    mainSizer->AddSpacer(20);

    this->SetSizer(mainSizer);

    GameModeDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK) {
        isPVPMode = dlg.IsPVPMode();
        turnLabel->SetLabel(isPVPMode ? "Current Turn: X" : "Player (X) vs AI (O)");
    }
}

void MainFrame::OnButtonHover(wxMouseEvent& evt) {
    wxButton* button = wxDynamicCast(evt.GetEventObject(), wxButton);
    if(button) {
        if(button == resetButton) {
            button->SetBackgroundColour(wxColor(100, 149, 237));  // Lighter blue
        } else if(button->GetLabel().empty() && !gameOver) {
            button->SetBackgroundColour(wxColor(220, 220, 255));  // Lighter hover
        }
        button->Refresh();
    }
    evt.Skip();
}

void MainFrame::OnButtonLeave(wxMouseEvent& evt) {
    wxButton* button = wxDynamicCast(evt.GetEventObject(), wxButton);
    if(button) {
        if(button == resetButton) {
            button->SetBackgroundColour(wxColor(65, 105, 225));  // Royal blue
        } else {
            button->SetBackgroundColour(wxColor(240, 240, 255));  // Original color
        }
        button->Refresh();
    }
    evt.Skip();
}

void MainFrame::OnButtonClick(wxCommandEvent& evt) {
    int id = evt.GetId() - 10000;
    int row = id / BOARD_SIZE;
    int col = id % BOARD_SIZE;

    if(gameOver || !board[row][col]->GetLabel().empty()) {
        return;
    }

    wxButton* button = board[row][col];

    if(isPVPMode) {
        // PvP Mode
        if(isXTurn) {
            button->SetLabel("X");
            button->SetForegroundColour(wxColor(0, 200, 0));     // Green
            button->SetBackgroundColour(wxColor(220, 255, 220)); // Light green
        } else {
            button->SetLabel("O");
            button->SetForegroundColour(wxColor(255, 0, 0));     // Red
            button->SetBackgroundColour(wxColor(255, 220, 220)); // Light red
        }
        button->Refresh();

        isXTurn = !isXTurn;
        turnLabel->SetLabel("Current Turn: " + wxString(isXTurn ? "X" : "O"));
        CheckWinner();
    } else {
        // PvAI Mode
        button->SetLabel("X");
        button->SetForegroundColour(wxColor(0, 200, 0));
        button->SetBackgroundColour(wxColor(220, 255, 220));
        button->Refresh();

        CheckWinner();
        if (!gameOver) {
            AIMove();
        }
    }

    evt.Skip();
}

void MainFrame::CheckWinner() {
    wxString winner;

    // Check rows
    for(int i = 0; i < BOARD_SIZE; ++i) {
        if(!board[i][0]->GetLabel().empty() &&
           board[i][0]->GetLabel() == board[i][1]->GetLabel() &&
           board[i][1]->GetLabel() == board[i][2]->GetLabel()) {
            winner = board[i][0]->GetLabel();
            // Highlight winning row
            for(int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j]->SetBackgroundColour(wxColor(255, 215, 0));  // Gold
                board[i][j]->Refresh();
            }
        }
    }

    // Check columns
    for(int i = 0; i < BOARD_SIZE; ++i) {
        if(!board[0][i]->GetLabel().empty() &&
           board[0][i]->GetLabel() == board[1][i]->GetLabel() &&
           board[1][i]->GetLabel() == board[2][i]->GetLabel()) {
            winner = board[0][i]->GetLabel();
            // Highlight winning column
            for(int j = 0; j < BOARD_SIZE; ++j) {
                board[j][i]->SetBackgroundColour(wxColor(255, 215, 0));  // Gold
                board[j][i]->Refresh();
            }
        }
    }

    // Check diagonals
    if(!board[0][0]->GetLabel().empty() &&
       board[0][0]->GetLabel() == board[1][1]->GetLabel() &&
       board[1][1]->GetLabel() == board[2][2]->GetLabel()) {
        winner = board[0][0]->GetLabel();
        // Highlight winning diagonal
        board[0][0]->SetBackgroundColour(wxColor(255, 215, 0));
        board[1][1]->SetBackgroundColour(wxColor(255, 215, 0));
        board[2][2]->SetBackgroundColour(wxColor(255, 215, 0));
        board[0][0]->Refresh();
        board[1][1]->Refresh();
        board[2][2]->Refresh();
    }

    if(!board[0][2]->GetLabel().empty() &&
       board[0][2]->GetLabel() == board[1][1]->GetLabel() &&
       board[1][1]->GetLabel() == board[2][0]->GetLabel()) {
        winner = board[0][2]->GetLabel();
        // Highlight winning diagonal
        board[0][2]->SetBackgroundColour(wxColor(255, 215, 0));
        board[1][1]->SetBackgroundColour(wxColor(255, 215, 0));
        board[2][0]->SetBackgroundColour(wxColor(255, 215, 0));
        board[0][2]->Refresh();
        board[1][1]->Refresh();
        board[2][0]->Refresh();
    }

    // Handle winner
    if(!winner.empty()) {
        if(winner == "X") {
            xScore++;
            xScoreLabel->SetLabel("X Score: " + std::to_string(xScore));
        } else {
            oScore++;
            oScoreLabel->SetLabel("O Score: " + std::to_string(oScore));
        }
        turnLabel->SetLabel("Game Over - " + winner + " Wins!");
        wxMessageBox(winner + " Wins!", "Game Over",
                    wxOK | wxICON_INFORMATION);
        gameOver = true;
        return;
    }

    // Check for draw
    bool isDraw = true;
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            if(board[i][j]->GetLabel().empty()) {
                isDraw = false;
                break;
            }
        }
        if(!isDraw) break;
    }

    if(isDraw) {
        turnLabel->SetLabel("Game Over - Draw!");
        wxMessageBox("It's a Draw!", "Game Over",
                    wxOK | wxICON_INFORMATION);
        gameOver = true;
    }
}

void MainFrame::ResetGame(wxCommandEvent& evt) {
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j]->SetLabel("");
            board[i][j]->SetBackgroundColour(wxColor(240, 240, 255));
            board[i][j]->SetForegroundColour(wxColor(50, 50, 50));
            board[i][j]->Refresh();
        }
    }

    isXTurn = true;
    gameOver = false;
    xScore = 0;
    oScore = 0;
    xScoreLabel->SetLabel("X Score: 0");
    oScoreLabel->SetLabel("O Score: 0");

    GameModeDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK) {
        isPVPMode = dlg.IsPVPMode();
        turnLabel->SetLabel(isPVPMode ? "Current Turn: X" : "Player (X) vs AI (O)");
    }

    evt.Skip();
}


void MainFrame::AIMove() {
    std::pair<int, int> bestMove = GetBestMove();
    int row = bestMove.first;
    int col = bestMove.second;

    wxButton* button = board[row][col];
    button->SetLabel("O");
    button->SetForegroundColour(wxColor(255, 0, 0));
    button->SetBackgroundColour(wxColor(255, 220, 220));
    button->Refresh();

    CheckWinner();
}

std::pair<int, int> MainFrame::GetBestMove() {
    int bestVal = -1000;
    std::pair<int, int> bestMove = {0, 0};

    // Evaluate all empty cells
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j]->GetLabel().empty()) {
                // Make the move
                board[i][j]->SetLabel("O");

                // Compute evaluation for this move
                int moveVal = Minimax(false, 0, -1000, 1000);

                // Undo the move
                board[i][j]->SetLabel("");

                if(moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove;
}

int MainFrame::Minimax(bool isMaximizing, int depth, int alpha, int beta) {
    int score = EvaluateBoard();

    // If Maximizer/Minimizer has won the game, return score
    if(score == 10 || score == -10)
        return score;

    // If no moves left, it's a tie
    if(!IsMovesLeft())
        return 0;

    if(isMaximizing) {
        int best = -1000;
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j]->GetLabel().empty()) {
                    board[i][j]->SetLabel("O");
                    best = std::max(best, Minimax(false, depth + 1, alpha, beta));
                    board[i][j]->SetLabel("");

                    alpha = std::max(alpha, best);
                    if(beta <= alpha)
                        break;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j]->GetLabel().empty()) {
                    board[i][j]->SetLabel("X");
                    best = std::min(best, Minimax(true, depth + 1, alpha, beta));
                    board[i][j]->SetLabel("");

                    beta = std::min(beta, best);
                    if(beta <= alpha)
                        break;
                }
            }
        }
        return best;
    }
}

int MainFrame::EvaluateBoard() {
    // Check rows
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(GetBoardState(i, 0) == GetBoardState(i, 1) &&
           GetBoardState(i, 1) == GetBoardState(i, 2)) {
            if(GetBoardState(i, 0) == "O")
                return 10;
            else if(GetBoardState(i, 0) == "X")
                return -10;
        }
    }

    // Check columns
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(GetBoardState(0, i) == GetBoardState(1, i) &&
           GetBoardState(1, i) == GetBoardState(2, i)) {
            if(GetBoardState(0, i) == "O")
                return 10;
            else if(GetBoardState(0, i) == "X")
                return -10;
        }
    }

    // Check diagonals
    if(GetBoardState(0, 0) == GetBoardState(1, 1) &&
       GetBoardState(1, 1) == GetBoardState(2, 2)) {
        if(GetBoardState(0, 0) == "O")
            return 10;
        else if(GetBoardState(0, 0) == "X")
            return -10;
    }

    if(GetBoardState(0, 2) == GetBoardState(1, 1) &&
       GetBoardState(1, 1) == GetBoardState(2, 0)) {
        if(GetBoardState(0, 2) == "O")
            return 10;
        else if(GetBoardState(0, 2) == "X")
            return -10;
    }

    return 0;
}

bool MainFrame::IsMovesLeft() {
    for(int i = 0; i < BOARD_SIZE; i++)
        for(int j = 0; j < BOARD_SIZE; j++)
            if(board[i][j]->GetLabel().empty())
                return true;
    return false;
}

wxString MainFrame::GetBoardState(int row, int col) {
    return board[row][col]->GetLabel();
}

