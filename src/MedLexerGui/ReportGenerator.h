#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>
#include <vector>
#include "Models.h"

class ReportGenerator {
public:
    void generatePatientsReport(
        const std::vector<Patient>& patients,
        const std::vector<Diagnosis>& diagnoses,
        const std::string& outputPath
    );

    void generateDoctorsReport(
        const std::vector<Doctor>& doctors,
        const std::vector<Appointment>& appointments,
        const std::string& outputPath
    );

    void generateAppointmentsReport(
        const std::vector<Appointment>& appointments,
        const std::string& outputPath
    );

    void generateGeneralReport(
        const std::vector<Patient>& patients,
        const std::vector<Doctor>& doctors,
        const std::vector<Appointment>& appointments,
        const std::vector<Diagnosis>& diagnoses,
        const std::string& outputPath
    );

    void generateGraphviz(
        const std::vector<Patient>& patients,
        const std::vector<Doctor>& doctors,
        const std::vector<Appointment>& appointments,
        const std::string& outputPath
    );

private:
    std::string getPatientStatus(const Patient& patient, const std::vector<Diagnosis>& diagnoses) const;
    int countAppointmentsForDoctor(const Doctor& doctor, const std::vector<Appointment>& appointments) const;
    int countUniquePatientsForDoctor(const Doctor& doctor, const std::vector<Appointment>& appointments) const;
    std::string getDoctorLoadLevel(int appointmentCount) const;
};

#endif
