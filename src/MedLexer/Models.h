#ifndef MODELS_H
#define MODELS_H

#include <string>

struct Patient {
    std::string name;
    int age;
    std::string bloodType;
    int room;
};

struct Doctor {
    std::string name;
    std::string specialty;
    std::string code;
};

struct Appointment {
    std::string patientName;
    std::string doctorName;
    std::string date;
    std::string time;
};

struct Diagnosis {
    std::string patientName;
    std::string condition;
    std::string medication;
    std::string dose;
};

#endif
