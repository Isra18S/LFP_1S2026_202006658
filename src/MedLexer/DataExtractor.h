#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

#include <vector>
#include <string>
#include "Token.h"
#include "Models.h"

class DataExtractor {
private:
    std::vector<Token> tokens;
    size_t pos;

public:
    DataExtractor(const std::vector<Token>& tokens);

    std::vector<Patient> getPatients();
    std::vector<Doctor> getDoctors();
    std::vector<Appointment> getAppointments();
    std::vector<Diagnosis> getDiagnoses();

private:
    Token currentToken() const;
    Token peekToken(int offset = 1) const;
    void advance();
    bool isAtEnd() const;

    std::string removeQuotes(const std::string& text) const;

    void skipUntilSection(const std::string& sectionName);
};

#endif
