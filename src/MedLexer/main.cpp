#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "LexicalAnalyzer.h"
#include "DataExtractor.h"
#include "ReportGenerator.h"

int main() {
    std::ifstream file("../../examples/ejemplo1.med");
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo de prueba." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    LexicalAnalyzer analyzer(content);
    std::vector<Token> tokens = analyzer.analyze();

    std::cout << "=== TOKENS ===" << std::endl;
    for (size_t i = 0; i < tokens.size(); i++) {
        std::cout << i + 1 << ". "
                  << tokens[i].lexeme << " | "
                  << tokenTypeToString(tokens[i].type) << " | "
                  << "Linea: " << tokens[i].line
                  << ", Columna: " << tokens[i].column
                  << std::endl;
    }

    std::cout << "\n=== ERRORES ===" << std::endl;
    const std::vector<LexicalError>& errors = analyzer.getErrors();
    for (size_t i = 0; i < errors.size(); i++) {
        std::cout << i + 1 << ". "
                  << errors[i].lexeme << " | "
                  << errors[i].type << " | "
                  << errors[i].description << " | "
                  << "Linea: " << errors[i].line
                  << ", Columna: " << errors[i].column
                  << " | Gravedad: " << errors[i].severity
                  << std::endl;
    }

    if (!errors.empty()) {
        std::cout << "\nSe encontraron errores lexicos. No se extraeran estructuras." << std::endl;
        return 0;
    }

    DataExtractor extractor(tokens);

    std::vector<Patient> patients = extractor.getPatients();
    std::vector<Doctor> doctors = extractor.getDoctors();
    std::vector<Appointment> appointments = extractor.getAppointments();
    std::vector<Diagnosis> diagnoses = extractor.getDiagnoses();

    std::cout << "\n=== PACIENTES ===" << std::endl;
    for (size_t i = 0; i < patients.size(); i++) {
        std::cout << i + 1 << ". "
                  << patients[i].name << " | Edad: " << patients[i].age
                  << " | Sangre: " << patients[i].bloodType
                  << " | Habitacion: " << patients[i].room
                  << std::endl;
    }

    std::cout << "\n=== MEDICOS ===" << std::endl;
    for (size_t i = 0; i < doctors.size(); i++) {
        std::cout << i + 1 << ". "
                  << doctors[i].name
                  << " | Especialidad: " << doctors[i].specialty
                  << " | Codigo: " << doctors[i].code
                  << std::endl;
    }

    std::cout << "\n=== CITAS ===" << std::endl;
    for (size_t i = 0; i < appointments.size(); i++) {
        std::cout << i + 1 << ". "
                  << appointments[i].patientName
                  << " | Medico: " << appointments[i].doctorName
                  << " | Fecha: " << appointments[i].date
                  << " | Hora: " << appointments[i].time
                  << std::endl;
    }

    std::cout << "\n=== DIAGNOSTICOS ===" << std::endl;
    for (size_t i = 0; i < diagnoses.size(); i++) {
        std::cout << i + 1 << ". "
                  << diagnoses[i].patientName
                  << " | Condicion: " << diagnoses[i].condition
                  << " | Medicamento: " << diagnoses[i].medication
                  << " | Dosis: " << diagnoses[i].dose
                  << std::endl;
    }

    std::cout << "\nDirectorio actual: " << std::filesystem::current_path() << std::endl;

    ReportGenerator generator;
    std::string reportPath = "../../reports/reporte_pacientes.html";
    generator.generatePatientsReport(patients, diagnoses, reportPath);

    std::cout << "Intentando generar reporte en: " << reportPath << std::endl;

    return 0;
}
