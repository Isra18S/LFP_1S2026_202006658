#include "ReportGenerator.h"
#include <fstream>
#include <sstream>

std::string ReportGenerator::getPatientStatus(const Patient& patient, const std::vector<Diagnosis>& diagnoses) const {
    int diagnosisCount = 0;

    for (size_t i = 0; i < diagnoses.size(); i++) {
        if (diagnoses[i].patientName == patient.name) {
            diagnosisCount++;
        }
    }

    if (diagnosisCount == 0) {
        return "SIN DIAG.";
    }
    else if (diagnosisCount > 1) {
        return "CRITICO";
    }
    else {
        return "ACTIVO";
    }
}

std::string ReportGenerator::getStatusClass(const std::string& status) const {
    if (status == "ACTIVO") return "activo";
    if (status == "SIN DIAG.") return "sin-diagn";
    if (status == "CRITICO") return "critico";
    return "";
}

void ReportGenerator::generatePatientsReport(
    const std::vector<Patient>& patients,
    const std::vector<Diagnosis>& diagnoses,
    const std::string& outputPath
) {
    std::ofstream file(outputPath.c_str());

    if (!file.is_open()) {
        return;
    }

    file << "<!DOCTYPE html>\n";
    file << "<html lang=\"es\">\n";
    file << "<head>\n";
    file << "    <meta charset=\"UTF-8\">\n";
    file << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    file << "    <title>Reporte de Pacientes</title>\n";
    file << "    <style>\n";
    file << "        body {\n";
    file << "            font-family: Arial, sans-serif;\n";
    file << "            margin: 20px;\n";
    file << "            background-color: #f4f6f8;\n";
    file << "        }\n";
    file << "        h1 {\n";
    file << "            color: #1f4e79;\n";
    file << "            text-align: center;\n";
    file << "        }\n";
    file << "        table {\n";
    file << "            width: 100%;\n";
    file << "            border-collapse: collapse;\n";
    file << "            background-color: white;\n";
    file << "        }\n";
    file << "        th, td {\n";
    file << "            border: 1px solid #ccc;\n";
    file << "            padding: 10px;\n";
    file << "            text-align: left;\n";
    file << "        }\n";
    file << "        th {\n";
    file << "            background-color: #1f4e79;\n";
    file << "            color: white;\n";
    file << "        }\n";
    file << "        .activo {\n";
    file << "            background-color: #d4edda;\n";
    file << "            color: #155724;\n";
    file << "            font-weight: bold;\n";
    file << "        }\n";
    file << "        .sin-diagn {\n";
    file << "            background-color: #fff3cd;\n";
    file << "            color: #856404;\n";
    file << "            font-weight: bold;\n";
    file << "        }\n";
    file << "        .critico {\n";
    file << "            background-color: #f8d7da;\n";
    file << "            color: #721c24;\n";
    file << "            font-weight: bold;\n";
    file << "        }\n";
    file << "    </style>\n";
    file << "</head>\n";
    file << "<body>\n";

    file << "    <h1>Historial de Pacientes</h1>\n";
    file << "    <table>\n";
    file << "        <tr>\n";
    file << "            <th>Paciente</th>\n";
    file << "            <th>Edad</th>\n";
    file << "            <th>Tipo de Sangre</th>\n";
    file << "            <th>Diagnostico Activo</th>\n";
    file << "            <th>Medicamento / Dosis</th>\n";
    file << "            <th>Estado</th>\n";
    file << "        </tr>\n";

    for (size_t i = 0; i < patients.size(); i++) {
        const Patient& patient = patients[i];

        std::string condition = "Sin diagnostico registrado";
        std::string medicationDose = "-";

        for (size_t j = 0; j < diagnoses.size(); j++) {
            if (diagnoses[j].patientName == patient.name) {
                condition = diagnoses[j].condition;
                medicationDose = diagnoses[j].medication + " / " + diagnoses[j].dose;
                break;
            }
        }

        std::string status = getPatientStatus(patient, diagnoses);
        std::string statusClass = getStatusClass(status);

        file << "        <tr>\n";
        file << "            <td>" << patient.name << "</td>\n";
        file << "            <td>" << patient.age << "</td>\n";
        file << "            <td>" << patient.bloodType << "</td>\n";
        file << "            <td>" << condition << "</td>\n";
        file << "            <td>" << medicationDose << "</td>\n";
        file << "            <td class=\"" << statusClass << "\">" << status << "</td>\n";
        file << "        </tr>\n";
    }

    file << "    </table>\n";
    file << "</body>\n";
    file << "</html>\n";

    file.close();
}
