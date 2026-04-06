#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "LexicalAnalyzer.h"
#include "DataExtractor.h"
#include "ReportGenerator.h"

// 🔥 ACTIVAR / DESACTIVAR DEBUG
#define DEBUG_MODE true

int main() {

    std::ifstream file("../../examples/ejemplo1.med");
    if (!file.is_open()) {
        std::cout << "Error al abrir archivo" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // ===================== LEXER =====================

    LexicalAnalyzer analyzer(content);
    std::vector<Token> tokens = analyzer.analyze();

    if (DEBUG_MODE) {
        std::cout << "=== TOKENS ===" << std::endl;
        for (size_t i = 0; i < tokens.size(); i++) {
            std::cout << i + 1 << ". "
                      << tokens[i].lexeme << " | "
                      << tokenTypeToString(tokens[i].type) << " | "
                      << "Linea: " << tokens[i].line
                      << ", Columna: " << tokens[i].column
                      << std::endl;
        }
    }

    // ===================== ERRORES =====================

    const std::vector<LexicalError>& errors = analyzer.getErrors();

    if (DEBUG_MODE) {
        std::cout << "\n=== ERRORES ===" << std::endl;

        if (errors.empty()) {
            std::cout << "Sin errores" << std::endl;
        }

        for (size_t i = 0; i < errors.size(); i++) {
            std::cout << i + 1 << ". "
                      << errors[i].lexeme << " | "
                      << errors[i].description << " | "
                      << "Linea: " << errors[i].line
                      << ", Columna: " << errors[i].column
                      << std::endl;
        }
    }

    if (!errors.empty()) {
        std::cout << "\nSe encontraron errores. No se generan reportes." << std::endl;
        return 0;
    }

    // ===================== EXTRACTOR =====================

    DataExtractor extractor(tokens);

    std::vector<Patient> patients = extractor.getPatients();
    std::vector<Doctor> doctors = extractor.getDoctors();
    std::vector<Appointment> appointments = extractor.getAppointments();
    std::vector<Diagnosis> diagnoses = extractor.getDiagnoses();

    // ===================== DEBUG DATOS =====================

    if (DEBUG_MODE) {

        std::cout << "\n=== PACIENTES ===" << std::endl;
        for (size_t i = 0; i < patients.size(); i++) {
            std::cout << i + 1 << ". "
                      << patients[i].name
                      << " | Edad: " << patients[i].age
                      << " | Sangre: " << patients[i].bloodType
                      << std::endl;
        }

        std::cout << "\n=== MEDICOS ===" << std::endl;
        for (size_t i = 0; i < doctors.size(); i++) {
            std::cout << i + 1 << ". "
                      << doctors[i].name
                      << " | " << doctors[i].specialty
                      << std::endl;
        }

        std::cout << "\n=== CITAS ===" << std::endl;
        for (size_t i = 0; i < appointments.size(); i++) {
            std::cout << i + 1 << ". "
                      << appointments[i].patientName
                      << " -> " << appointments[i].doctorName
                      << " (" << appointments[i].date
                      << " " << appointments[i].time << ")"
                      << std::endl;
        }

        std::cout << "\n=== DIAGNOSTICOS ===" << std::endl;
        for (size_t i = 0; i < diagnoses.size(); i++) {
            std::cout << i + 1 << ". "
                      << diagnoses[i].patientName
                      << " | " << diagnoses[i].condition
                      << std::endl;
        }
    }

    // ===================== REPORTES =====================

    ReportGenerator g;

    g.generatePatientsReport(patients, diagnoses, "../../reports/pacientes.html");
    g.generateDoctorsReport(doctors, appointments, "../../reports/medicos.html");
    g.generateAppointmentsReport(appointments, "../../reports/citas.html");
    g.generateGeneralReport(patients, doctors, appointments, diagnoses, "../../reports/general.html");
    g.generateGraphviz(patients, doctors, appointments, "../../reports/grafo.dot");

    std::cout << "\n=== PROCESO COMPLETADO ===" << std::endl;
    std::cout << "Reportes generados en carpeta /reports" << std::endl;

    return 0;
}
