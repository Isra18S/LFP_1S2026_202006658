#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <vector>
#include "LexicalAnalyzer.h"
#include "DataExtractor.h"
#include "ReportGenerator.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    wxTextCtrl* txtEditor;
    wxGrid* gridTokens;
    wxGrid* gridErrors;
    wxButton* btnLoad;
    wxButton* btnAnalyze;
    wxButton* btnOpenReports;
    wxString currentFilePath;

    void OnLoadFile(wxCommandEvent& event);
    void OnAnalyze(wxCommandEvent& event);
    void OnOpenReports(wxCommandEvent& event);

    void FillTokenGrid(const std::vector<Token>& tokens);
    void FillErrorGrid(const std::vector<LexicalError>& errors);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BTN_LOAD = 1001,
    ID_BTN_ANALYZE,
    ID_BTN_OPEN_REPORTS
};

#endif
