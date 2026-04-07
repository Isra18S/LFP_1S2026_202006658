#include "ReportGenerator.h"
#include <fstream>
#include <set>
#include <iostream>

std::string ReportGenerator::getPatientStatus(const Patient& p, const std::vector<Diagnosis>& d) const {
    int count = 0;
    for (size_t i = 0; i < d.size(); i++) {
        if (d[i].patientName == p.name) count++;
    }

    if (count == 0) return "SIN DIAG.";
    if (count > 1) return "CRITICO";
    return "ACTIVO";
}

void ReportGenerator::generatePatientsReport(
    const std::vector<Patient>& patients,
    const std::vector<Diagnosis>& diagnoses,
    const std::string& path
) {
    std::ofstream f(path.c_str());

    f << "<html><body><h1>Pacientes</h1><table border='1'>";
    f << "<tr><th>Nombre</th><th>Edad</th><th>Sangre</th><th>Diagnostico</th><th>Medicamento</th><th>Estado</th></tr>";

    for (size_t i = 0; i < patients.size(); i++) {
        const Patient& p = patients[i];
        std::string condition = "Sin diagnostico";
        std::string medication = "-";

        for (size_t j = 0; j < diagnoses.size(); j++) {
            if (diagnoses[j].patientName == p.name) {
                condition = diagnoses[j].condition;
                medication = diagnoses[j].medication + " / " + diagnoses[j].dose;
                break;
            }
        }

        f << "<tr><td>" << p.name << "</td><td>" << p.age
          << "</td><td>" << p.bloodType << "</td><td>"
          << condition << "</td><td>" << medication << "</td><td>"
          << getPatientStatus(p, diagnoses) << "</td></tr>";
    }

    f << "</table></body></html>";
}

int ReportGenerator::countAppointmentsForDoctor(const Doctor& d, const std::vector<Appointment>& a) const {
    int c = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].doctorName == d.name) c++;
    }
    return c;
}

int ReportGenerator::countUniquePatientsForDoctor(const Doctor& d, const std::vector<Appointment>& a) const {
    std::set<std::string> s;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].doctorName == d.name) s.insert(a[i].patientName);
    }
    return (int)s.size();
}

std::string ReportGenerator::getDoctorLoadLevel(int c) const {
    if (c <= 3) return "BAJA";
    if (c <= 6) return "NORMAL";
    if (c <= 8) return "ALTA";
    return "SATURADA";
}

void ReportGenerator::generateDoctorsReport(
    const std::vector<Doctor>& doctors,
    const std::vector<Appointment>& appointments,
    const std::string& path
) {
    std::ofstream f(path.c_str());

    f << "<html><body><h1>Medicos</h1><table border='1'>";
    f << "<tr><th>Nombre</th><th>Codigo</th><th>Especialidad</th><th>Citas</th><th>Pacientes</th><th>Carga</th></tr>";

    for (size_t i = 0; i < doctors.size(); i++) {
        const Doctor& d = doctors[i];
        int c = countAppointmentsForDoctor(d, appointments);
        int p = countUniquePatientsForDoctor(d, appointments);

        f << "<tr><td>" << d.name << "</td><td>" << d.code
          << "</td><td>" << d.specialty << "</td><td>"
          << c << "</td><td>" << p << "</td><td>"
          << getDoctorLoadLevel(c) << "</td></tr>";
    }

    f << "</table></body></html>";
}

void ReportGenerator::generateAppointmentsReport(
    const std::vector<Appointment>& a,
    const std::string& path
) {
    std::ofstream f(path.c_str());

    f << "<html><body><h1>Citas</h1><table border='1'>";
    f << "<tr><th>Paciente</th><th>Medico</th><th>Fecha</th><th>Hora</th><th>Estado</th></tr>";

    for (size_t i = 0; i < a.size(); i++) {
        bool conflicto = false;

        for (size_t j = 0; j < a.size(); j++) {
            if (i != j &&
                a[i].doctorName == a[j].doctorName &&
                a[i].date == a[j].date &&
                a[i].time == a[j].time) {
                conflicto = true;
            }
        }

        f << "<tr><td>" << a[i].patientName << "</td><td>"
          << a[i].doctorName << "</td><td>"
          << a[i].date << "</td><td>"
          << a[i].time << "</td><td>"
          << (conflicto ? "CONFLICTO" : "OK") << "</td></tr>";
    }

    f << "</table></body></html>";
}

void ReportGenerator::generateGeneralReport(
    const std::vector<Patient>& p,
    const std::vector<Doctor>& d,
    const std::vector<Appointment>& a,
    const std::vector<Diagnosis>& di,
    const std::string& path
) {
    std::ofstream f(path.c_str());

    f << "<html><body><h1>General</h1>";
    f << "<p>Pacientes: " << p.size() << "</p>";
    f << "<p>Medicos: " << d.size() << "</p>";
    f << "<p>Citas: " << a.size() << "</p>";
    f << "<p>Diagnosticos: " << di.size() << "</p>";
    f << "</body></html>";
}

void ReportGenerator::generateGraphviz(
    const std::vector<Patient>& p,
    const std::vector<Doctor>& d,
    const std::vector<Appointment>& a,
    const std::string& path
) {
    std::ofstream f(path.c_str());

    f << "digraph G {\n";

    for (size_t i = 0; i < p.size(); i++) {
        f << "\"" << p[i].name << "\";\n";
    }

    for (size_t i = 0; i < d.size(); i++) {
        f << "\"" << d[i].name << "\";\n";
    }

    for (size_t i = 0; i < a.size(); i++) {
        f << "\"" << a[i].patientName << "\" -> \"" << a[i].doctorName << "\";\n";
    }

    f << "}\n";
}
