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

private:
    std::string getPatientStatus(const Patient& patient, const std::vector<Diagnosis>& diagnoses) const;
    std::string getStatusClass(const std::string& status) const;
};

#endif
