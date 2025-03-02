// GameModeDialog.cpp
#include "GameModeDialog.h"

wxBEGIN_EVENT_TABLE(GameModeDialog, wxDialog)
wxEND_EVENT_TABLE()

GameModeDialog::GameModeDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Select Game Mode", wxDefaultPosition, wxSize(300, 200)) {

    this->SetBackgroundColour(wxColor(44, 44, 84));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title
    wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "Select Game Mode",
                                              wxDefaultPosition, wxDefaultSize,
                                              wxALIGN_CENTER_HORIZONTAL);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(16);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColor(255, 255, 255));

    // Buttons
    wxFont buttonFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    pvpButton = new wxButton(this, wxID_ANY, "Player vs Player",
                           wxDefaultPosition, wxSize(200, 50));
    pvpButton->SetFont(buttonFont);
    pvpButton->SetBackgroundColour(wxColor(65, 105, 225));
    pvpButton->SetForegroundColour(wxColor(255, 255, 255));

    pvaiButton = new wxButton(this, wxID_ANY, "Player vs AI",
                            wxDefaultPosition, wxSize(200, 50));
    pvaiButton->SetFont(buttonFont);
    pvaiButton->SetBackgroundColour(wxColor(65, 105, 225));
    pvaiButton->SetForegroundColour(wxColor(255, 255, 255));

    pvpButton->Bind(wxEVT_BUTTON, &GameModeDialog::OnModeSelect, this);
    pvaiButton->Bind(wxEVT_BUTTON, &GameModeDialog::OnModeSelect, this);

    // Layout
    mainSizer->AddSpacer(20);
    mainSizer->Add(titleText, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddSpacer(10);
    mainSizer->Add(pvpButton, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(pvaiButton, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->AddSpacer(20);

    SetSizer(mainSizer);
}

void GameModeDialog::OnModeSelect(wxCommandEvent& evt) {
    if (evt.GetEventObject() == pvpButton) {
        isPVPMode = true;
    } else {
        isPVPMode = false;
    }
    EndModal(wxID_OK);
}