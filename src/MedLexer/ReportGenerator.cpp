#include "ReportGenerator.h"
#include <fstream>
#include <set>
#include <iostream>

// ================= PACIENTES =================

std::string ReportGenerator::getPatientStatus(const Patient& p, const std::vector<Diagnosis>& d) const {
    int count = 0;
    for (auto& x : d) if (x.patientName == p.name) count++;

    if (count == 0) return "SIN DIAG.";
    if (count > 1) return "CRITICO";
    return "ACTIVO";
}

void ReportGenerator::generatePatientsReport(
    const std::vector<Patient>& patients,
    const std::vector<Diagnosis>& diagnoses,
    const std::string& path
) {
    std::ofstream f(path);

    f << "<html><body><h1>Pacientes</h1><table border='1'>";
    f << "<tr><th>Nombre</th><th>Edad</th><th>Sangre</th><th>Estado</th></tr>";

    for (auto& p : patients) {
        f << "<tr><td>" << p.name << "</td><td>" << p.age
          << "</td><td>" << p.bloodType << "</td><td>"
          << getPatientStatus(p, diagnoses) << "</td></tr>";
    }

    f << "</table></body></html>";
}

// ================= MEDICOS =================

int ReportGenerator::countAppointmentsForDoctor(const Doctor& d, const std::vector<Appointment>& a) const {
    int c = 0;
    for (auto& x : a) if (x.doctorName == d.name) c++;
    return c;
}

int ReportGenerator::countUniquePatientsForDoctor(const Doctor& d, const std::vector<Appointment>& a) const {
    std::set<std::string> s;
    for (auto& x : a) if (x.doctorName == d.name) s.insert(x.patientName);
    return s.size();
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
    std::ofstream f(path);

    f << "<html><body><h1>Medicos</h1><table border='1'>";
    f << "<tr><th>Nombre</th><th>Citas</th><th>Pacientes</th><th>Carga</th></tr>";

    for (auto& d : doctors) {
        int c = countAppointmentsForDoctor(d, appointments);
        int p = countUniquePatientsForDoctor(d, appointments);

        f << "<tr><td>" << d.name << "</td><td>" << c
          << "</td><td>" << p << "</td><td>"
          << getDoctorLoadLevel(c) << "</td></tr>";
    }

    f << "</table></body></html>";
}

// ================= CITAS =================

void ReportGenerator::generateAppointmentsReport(
    const std::vector<Appointment>& a,
    const std::string& path
) {
    std::ofstream f(path);

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

// ================= GENERAL =================

void ReportGenerator::generateGeneralReport(
    const std::vector<Patient>& p,
    const std::vector<Doctor>& d,
    const std::vector<Appointment>& a,
    const std::vector<Diagnosis>& di,
    const std::string& path
) {
    std::ofstream f(path);

    f << "<html><body><h1>General</h1>";
    f << "<p>Pacientes: " << p.size() << "</p>";
    f << "<p>Medicos: " << d.size() << "</p>";
    f << "<p>Citas: " << a.size() << "</p>";
    f << "<p>Diagnosticos: " << di.size() << "</p>";
    f << "</body></html>";
}

// ================= GRAPHVIZ =================

void ReportGenerator::generateGraphviz(
    const std::vector<Patient>& p,
    const std::vector<Doctor>& d,
    const std::vector<Appointment>& a,
    const std::string& path
) {
    std::ofstream f(path);

    f << "digraph G {\n";

    for (auto& x : p) {
        f << "\"" << x.name << "\";\n";
    }

    for (auto& x : d) {
        f << "\"" << x.name << "\";\n";
    }

    for (auto& x : a) {
        f << "\"" << x.patientName << "\" -> \"" << x.doctorName << "\";\n";
    }

    f << "}\n";
}
