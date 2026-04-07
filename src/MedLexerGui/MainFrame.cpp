#include "MainFrame.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_BTN_LOAD, MainFrame::OnLoadFile)
    EVT_BUTTON(ID_BTN_ANALYZE, MainFrame::OnAnalyze)
    EVT_BUTTON(ID_BTN_OPEN_REPORTS, MainFrame::OnOpenReports)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 700)) {

    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    btnLoad = new wxButton(panel, ID_BTN_LOAD, "Cargar .med");
    btnAnalyze = new wxButton(panel, ID_BTN_ANALYZE, "Analizar");
    btnOpenReports = new wxButton(panel, ID_BTN_OPEN_REPORTS, "Abrir Reportes");

    topSizer->Add(btnLoad, 0, wxALL, 5);
    topSizer->Add(btnAnalyze, 0, wxALL, 5);
    topSizer->Add(btnOpenReports, 0, wxALL, 5);

    txtEditor = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(500, 500),
                               wxTE_MULTILINE | wxTE_RICH2);

    wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);

    wxPanel* tokenPanel = new wxPanel(notebook);
    wxBoxSizer* tokenSizer = new wxBoxSizer(wxVERTICAL);
    gridTokens = new wxGrid(tokenPanel, wxID_ANY);
    gridTokens->CreateGrid(0, 5);
    gridTokens->SetColLabelValue(0, "No.");
    gridTokens->SetColLabelValue(1, "Lexema");
    gridTokens->SetColLabelValue(2, "Tipo");
    gridTokens->SetColLabelValue(3, "Linea");
    gridTokens->SetColLabelValue(4, "Columna");
    tokenSizer->Add(gridTokens, 1, wxEXPAND | wxALL, 5);
    tokenPanel->SetSizer(tokenSizer);

    wxPanel* errorPanel = new wxPanel(notebook);
    wxBoxSizer* errorSizer = new wxBoxSizer(wxVERTICAL);
    gridErrors = new wxGrid(errorPanel, wxID_ANY);
    gridErrors->CreateGrid(0, 6);
    gridErrors->SetColLabelValue(0, "No.");
    gridErrors->SetColLabelValue(1, "Lexema");
    gridErrors->SetColLabelValue(2, "Tipo");
    gridErrors->SetColLabelValue(3, "Descripcion");
    gridErrors->SetColLabelValue(4, "Linea");
    gridErrors->SetColLabelValue(5, "Columna");
    errorSizer->Add(gridErrors, 1, wxEXPAND | wxALL, 5);
    errorPanel->SetSizer(errorSizer);

    notebook->AddPage(tokenPanel, "Tokens");
    notebook->AddPage(errorPanel, "Errores");

    bottomSizer->Add(txtEditor, 1, wxEXPAND | wxALL, 5);
    bottomSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

    mainSizer->Add(topSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(bottomSizer, 1, wxEXPAND | wxALL, 5);

    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrame::OnLoadFile(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Abrir archivo .med", "", "",
                                "Archivos MED (*.med)|*.med",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    currentFilePath = openFileDialog.GetPath();

    std::ifstream file(currentFilePath.ToStdString());
    if (!file.is_open()) {
        wxMessageBox("No se pudo abrir el archivo.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    txtEditor->SetValue(wxString::FromUTF8(buffer.str()));
}

void MainFrame::OnAnalyze(wxCommandEvent& event) {
    std::string content = txtEditor->GetValue().ToStdString();

    LexicalAnalyzer analyzer(content);
    std::vector<Token> tokens = analyzer.analyze();
    const std::vector<LexicalError>& errors = analyzer.getErrors();

    FillTokenGrid(tokens);
    FillErrorGrid(errors);

    if (!errors.empty()) {
        wxMessageBox("Se encontraron errores léxicos. No se generarán reportes.", "Aviso", wxOK | wxICON_WARNING);
        return;
    }

    DataExtractor extractor(tokens);
    std::vector<Patient> patients = extractor.getPatients();
    std::vector<Doctor> doctors = extractor.getDoctors();
    std::vector<Appointment> appointments = extractor.getAppointments();
    std::vector<Diagnosis> diagnoses = extractor.getDiagnoses();

    ReportGenerator g;
    g.generatePatientsReport(patients, diagnoses, "../../reports/pacientes.html");
    g.generateDoctorsReport(doctors, appointments, "../../reports/medicos.html");
    g.generateAppointmentsReport(appointments, "../../reports/citas.html");
    g.generateGeneralReport(patients, doctors, appointments, diagnoses, "../../reports/general.html");
    g.generateGraphviz(patients, doctors, appointments, "../../reports/grafo.dot");

    wxMessageBox("Analisis completado y reportes generados.", "Exito", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenReports(wxCommandEvent& event) {
#ifdef _WIN32
    system("explorer ../../reports");
#else
    system("xdg-open ../../reports");
#endif
}

void MainFrame::FillTokenGrid(const std::vector<Token>& tokens) {
    if (gridTokens->GetNumberRows() > 0) {
        gridTokens->DeleteRows(0, gridTokens->GetNumberRows());
    }

    if (!tokens.empty()) {
        gridTokens->AppendRows(tokens.size());
    }

    for (size_t i = 0; i < tokens.size(); i++) {
        gridTokens->SetCellValue(i, 0, wxString::Format("%d", (int)i + 1));
        gridTokens->SetCellValue(i, 1, wxString::FromUTF8(tokens[i].lexeme));
        gridTokens->SetCellValue(i, 2, wxString::FromUTF8(tokenTypeToString(tokens[i].type)));
        gridTokens->SetCellValue(i, 3, wxString::Format("%d", tokens[i].line));
        gridTokens->SetCellValue(i, 4, wxString::Format("%d", tokens[i].column));
    }
}

void MainFrame::FillErrorGrid(const std::vector<LexicalError>& errors) {
    if (gridErrors->GetNumberRows() > 0) {
        gridErrors->DeleteRows(0, gridErrors->GetNumberRows());
    }

    if (!errors.empty()) {
        gridErrors->AppendRows(errors.size());
    }

    for (size_t i = 0; i < errors.size(); i++) {
        gridErrors->SetCellValue(i, 0, wxString::Format("%d", (int)i + 1));
        gridErrors->SetCellValue(i, 1, wxString::FromUTF8(errors[i].lexeme));
        gridErrors->SetCellValue(i, 2, wxString::FromUTF8(errors[i].type));
        gridErrors->SetCellValue(i, 3, wxString::FromUTF8(errors[i].description));
        gridErrors->SetCellValue(i, 4, wxString::Format("%d", errors[i].line));
        gridErrors->SetCellValue(i, 5, wxString::Format("%d", errors[i].column));
    }
}
