#include "DataExtractor.h"

DataExtractor::DataExtractor(const std::vector<Token>& tokens)
    : tokens(tokens), pos(0) {}

Token DataExtractor::currentToken() const {
    if (pos < tokens.size()) {
        return tokens[pos];
    }
    return Token(TokenType::END_OF_FILE, "EOF", -1, -1);
}

Token DataExtractor::peekToken(int offset) const {
    if (pos + offset < tokens.size()) {
        return tokens[pos + offset];
    }
    return Token(TokenType::END_OF_FILE, "EOF", -1, -1);
}

void DataExtractor::advance() {
    if (pos < tokens.size()) {
        pos++;
    }
}

bool DataExtractor::isAtEnd() const {
    return pos >= tokens.size() || currentToken().type == TokenType::END_OF_FILE;
}

std::string DataExtractor::removeQuotes(const std::string& text) const {
    if (text.size() >= 2 && text.front() == '"' && text.back() == '"') {
        return text.substr(1, text.size() - 2);
    }
    return text;
}

void DataExtractor::skipUntilSection(const std::string& sectionName) {
    pos = 0;
    while (!isAtEnd() && currentToken().lexeme != sectionName) {
        advance();
    }
}

std::vector<Patient> DataExtractor::getPatients() {
    std::vector<Patient> patients;
    skipUntilSection("PACIENTES");

    while (!isAtEnd()) {
        if (currentToken().lexeme == "MEDICOS") break;

        if (currentToken().lexeme == "paciente") {
            Patient p;
            p.name = "";
            p.age = 0;
            p.bloodType = "";
            p.room = 0;

            advance(); // paciente
            if (currentToken().lexeme == ":") advance();

            if (currentToken().type == TokenType::STRING) {
                p.name = removeQuotes(currentToken().lexeme);
                advance();
            }

            if (currentToken().lexeme == "[") advance();

            while (!isAtEnd() && currentToken().lexeme != "]") {
                if (currentToken().lexeme == "edad") {
                    advance(); // edad
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::INTEGER) {
                        p.age = std::stoi(currentToken().lexeme);
                        advance();
                    }
                }
                else if (currentToken().lexeme == "tipo_sangre") {
                    advance(); // tipo_sangre
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::BLOOD_TYPE || currentToken().type == TokenType::STRING) {
                        p.bloodType = removeQuotes(currentToken().lexeme);
                        advance();
                    }
                }
                else if (currentToken().lexeme == "habitacion") {
                    advance(); // habitacion
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::INTEGER) {
                        p.room = std::stoi(currentToken().lexeme);
                        advance();
                    }
                }
                else {
                    advance();
                }

                if (currentToken().lexeme == ",") {
                    advance();
                }
            }

            if (currentToken().lexeme == "]") advance();
            if (currentToken().lexeme == ",") advance();

            patients.push_back(p);
        }
        else {
            advance();
        }
    }

    return patients;
}

std::vector<Doctor> DataExtractor::getDoctors() {
    std::vector<Doctor> doctors;
    skipUntilSection("MEDICOS");

    while (!isAtEnd()) {
        if (currentToken().lexeme == "CITAS") break;

        if (currentToken().lexeme == "medico") {
            Doctor d;
            d.name = "";
            d.specialty = "";
            d.code = "";

            advance(); // medico
            if (currentToken().lexeme == ":") advance();

            if (currentToken().type == TokenType::STRING) {
                d.name = removeQuotes(currentToken().lexeme);
                advance();
            }

            if (currentToken().lexeme == "[") advance();

            while (!isAtEnd() && currentToken().lexeme != "]") {
                if (currentToken().lexeme == "especialidad") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::SPECIALTY) {
                        d.specialty = currentToken().lexeme;
                        advance();
                    }
                }
                else if (currentToken().lexeme == "codigo") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::CODE_ID || currentToken().type == TokenType::STRING) {
                        d.code = removeQuotes(currentToken().lexeme);
                        advance();
                    }
                }
                else {
                    advance();
                }

                if (currentToken().lexeme == ",") {
                    advance();
                }
            }

            if (currentToken().lexeme == "]") advance();
            if (currentToken().lexeme == ",") advance();

            doctors.push_back(d);
        }
        else {
            advance();
        }
    }

    return doctors;
}

std::vector<Appointment> DataExtractor::getAppointments() {
    std::vector<Appointment> appointments;
    skipUntilSection("CITAS");

    while (!isAtEnd()) {
        if (currentToken().lexeme == "DIAGNOSTICOS") break;

        if (currentToken().lexeme == "cita") {
            Appointment a;
            a.patientName = "";
            a.doctorName = "";
            a.date = "";
            a.time = "";

            advance(); // cita
            if (currentToken().lexeme == ":") advance();

            if (currentToken().type == TokenType::STRING) {
                a.patientName = removeQuotes(currentToken().lexeme);
                advance();
            }

            if (currentToken().lexeme == "con") advance();

            if (currentToken().type == TokenType::STRING) {
                a.doctorName = removeQuotes(currentToken().lexeme);
                advance();
            }

            if (currentToken().lexeme == "[") advance();

            while (!isAtEnd() && currentToken().lexeme != "]") {
                if (currentToken().lexeme == "fecha") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::DATE) {
                        a.date = currentToken().lexeme;
                        advance();
                    }
                }
                else if (currentToken().lexeme == "hora") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::TIME) {
                        a.time = currentToken().lexeme;
                        advance();
                    }
                }
                else {
                    advance();
                }

                if (currentToken().lexeme == ",") {
                    advance();
                }
            }

            if (currentToken().lexeme == "]") advance();
            if (currentToken().lexeme == ",") advance();

            appointments.push_back(a);
        }
        else {
            advance();
        }
    }

    return appointments;
}

std::vector<Diagnosis> DataExtractor::getDiagnoses() {
    std::vector<Diagnosis> diagnoses;
    skipUntilSection("DIAGNOSTICOS");

    while (!isAtEnd()) {
        if (currentToken().type == TokenType::END_OF_FILE) break;

        if (currentToken().lexeme == "diagnostico") {
            Diagnosis d;
            d.patientName = "";
            d.condition = "";
            d.medication = "";
            d.dose = "";

            advance(); // diagnostico
            if (currentToken().lexeme == ":") advance();

            if (currentToken().type == TokenType::STRING) {
                d.patientName = removeQuotes(currentToken().lexeme);
                advance();
            }

            if (currentToken().lexeme == "[") advance();

            while (!isAtEnd() && currentToken().lexeme != "]") {
                if (currentToken().lexeme == "condicion") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::STRING) {
                        d.condition = removeQuotes(currentToken().lexeme);
                        advance();
                    }
                }
                else if (currentToken().lexeme == "medicamento") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::STRING) {
                        d.medication = removeQuotes(currentToken().lexeme);
                        advance();
                    }
                }
                else if (currentToken().lexeme == "dosis") {
                    advance();
                    if (currentToken().lexeme == ":") advance();
                    if (currentToken().type == TokenType::DOSE) {
                        d.dose = currentToken().lexeme;
                        advance();
                    }
                }
                else {
                    advance();
                }

                if (currentToken().lexeme == ",") {
                    advance();
                }
            }

            if (currentToken().lexeme == "]") advance();
            if (currentToken().lexeme == ",") advance();

            diagnoses.push_back(d);
        }
        else {
            advance();
        }
    }

    return diagnoses;
}
