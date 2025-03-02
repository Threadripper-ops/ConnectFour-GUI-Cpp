// GameModeDialog.h
#ifndef GAMEMODEDIALOG_H
#define GAMEMODEDIALOG_H

#include <wx/wx.h>

class GameModeDialog : public wxDialog {
public:
    GameModeDialog(wxWindow* parent);
    bool IsPVPMode() const { return isPVPMode; }

private:
    void OnModeSelect(wxCommandEvent& evt);
    bool isPVPMode;
    wxButton* pvpButton;
    wxButton* pvaiButton;

    wxDECLARE_EVENT_TABLE();
};

#endif