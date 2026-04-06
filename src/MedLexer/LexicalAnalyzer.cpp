#include "LexicalAnalyzer.h"
#include <cctype>
#include <sstream>

LexicalAnalyzer::LexicalAnalyzer(const std::string& text)
    : input(text), pos(0), line(1), column(1) {}

const std::vector<LexicalError>& LexicalAnalyzer::getErrors() const {
    return errorManager.getErrors();
}

char LexicalAnalyzer::currentChar() const {
    if (pos >= input.size()) return '\0';
    return input[pos];
}

char LexicalAnalyzer::peekChar() const {
    if (pos + 1 >= input.size()) return '\0';
    return input[pos + 1];
}

void LexicalAnalyzer::advance() {
    if (pos < input.size()) {
        if (input[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
}

void LexicalAnalyzer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(currentChar()))) {
        advance();
    }
}

bool LexicalAnalyzer::isDelimiter(char c) const {
    return c == '{' || c == '}' || c == '[' || c == ']' ||
           c == ':' || c == ',' || c == ';';
}

bool LexicalAnalyzer::isSection(const std::string& lexeme) const {
    return lexeme == "HOSPITAL" ||
           lexeme == "PACIENTES" ||
           lexeme == "MEDICOS" ||
           lexeme == "CITAS" ||
           lexeme == "DIAGNOSTICOS";
}

bool LexicalAnalyzer::isElement(const std::string& lexeme) const {
    return lexeme == "paciente" ||
           lexeme == "medico" ||
           lexeme == "cita" ||
           lexeme == "diagnostico";
}

bool LexicalAnalyzer::isAttribute(const std::string& lexeme) const {
    return lexeme == "edad" ||
           lexeme == "tipo_sangre" ||
           lexeme == "habitacion" ||
           lexeme == "especialidad" ||
           lexeme == "codigo" ||
           lexeme == "fecha" ||
           lexeme == "hora" ||
           lexeme == "condicion" ||
           lexeme == "medicamento" ||
           lexeme == "dosis" ||
           lexeme == "con";
}

bool LexicalAnalyzer::isSpecialty(const std::string& lexeme) const {
    return lexeme == "CARDIOLOGIA" ||
           lexeme == "NEUROLOGIA" ||
           lexeme == "PEDIATRIA" ||
           lexeme == "CIRUGIA" ||
           lexeme == "MEDICINA_GENERAL" ||
           lexeme == "ONCOLOGIA";
}

bool LexicalAnalyzer::isDose(const std::string& lexeme) const {
    return lexeme == "DIARIA" ||
           lexeme == "CADA_8_HORAS" ||
           lexeme == "CADA_12_HORAS" ||
           lexeme == "SEMANAL";
}

bool LexicalAnalyzer::isBloodType(const std::string& lexeme) const {
    return lexeme == "A+" || lexeme == "A-" ||
           lexeme == "B+" || lexeme == "B-" ||
           lexeme == "O+" || lexeme == "O-" ||
           lexeme == "AB+" || lexeme == "AB-";
}

bool LexicalAnalyzer::isValidDate(const std::string& lexeme) const {
    if (lexeme.size() != 10) return false;
    if (lexeme[4] != '-' || lexeme[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(lexeme[i]))) return false;
    }

    int month = std::stoi(lexeme.substr(5, 2));
    int day = std::stoi(lexeme.substr(8, 2));

    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

bool LexicalAnalyzer::isValidTime(const std::string& lexeme) const {
    if (lexeme.size() != 5) return false;
    if (lexeme[2] != ':') return false;

    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (!std::isdigit(static_cast<unsigned char>(lexeme[i]))) return false;
    }

    int hour = std::stoi(lexeme.substr(0, 2));
    int minute = std::stoi(lexeme.substr(3, 2));

    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

bool LexicalAnalyzer::isCodeId(const std::string& lexeme) const {
    if (lexeme.size() < 5) return false;

    size_t dashPos = lexeme.find('-');
    if (dashPos == std::string::npos) return false;
    if (dashPos != 3) return false;

    for (int i = 0; i < 3; i++) {
        if (!std::isalpha(static_cast<unsigned char>(lexeme[i]))) {
            return false;
        }
    }

    for (size_t i = 4; i < lexeme.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(lexeme[i]))) {
            return false;
        }
    }

    return true;
}

std::vector<Token> LexicalAnalyzer::analyze() {
    std::vector<Token> tokens;
    Token token;

    do {
        token = nextToken();
        tokens.push_back(token);
    } while (token.type != TokenType::END_OF_FILE);

    return tokens;
}

Token LexicalAnalyzer::nextToken() {
    skipWhitespace();

    int startLine = line;
    int startColumn = column;
    char c = currentChar();

    if (c == '\0') {
        return Token(TokenType::END_OF_FILE, "EOF", startLine, startColumn);
    }

    if (isDelimiter(c)) {
        return readDelimiter();
    }

    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
        return readWord();
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
        return readNumberOrDateOrTime();
    }

    if (c == '"') {
        return readString();
    }

    return makeUnknownToken();
}

Token LexicalAnalyzer::readDelimiter() {
    int startLine = line;
    int startColumn = column;
    std::string lexeme(1, currentChar());
    advance();
    return Token(TokenType::DELIMITER, lexeme, startLine, startColumn);
}

Token LexicalAnalyzer::readWord() {
    int startLine = line;
    int startColumn = column;
    std::string lexeme;

    while (std::isalnum(static_cast<unsigned char>(currentChar())) || currentChar() == '_') {
        lexeme += currentChar();
        advance();
    }

    if (isSection(lexeme)) return Token(TokenType::SECTION, lexeme, startLine, startColumn);
    if (isElement(lexeme)) return Token(TokenType::ELEMENT, lexeme, startLine, startColumn);
    if (isAttribute(lexeme)) return Token(TokenType::ATTRIBUTE, lexeme, startLine, startColumn);
    if (isSpecialty(lexeme)) return Token(TokenType::SPECIALTY, lexeme, startLine, startColumn);
    if (isDose(lexeme)) return Token(TokenType::DOSE, lexeme, startLine, startColumn);

    return Token(TokenType::UNKNOWN, lexeme, startLine, startColumn);
}

Token LexicalAnalyzer::readNumberOrDateOrTime() {
    int startLine = line;
    int startColumn = column;
    std::string lexeme;

    while (std::isdigit(static_cast<unsigned char>(currentChar())) ||
           currentChar() == '-' || currentChar() == ':') {
        lexeme += currentChar();
        advance();
    }

    if (isValidDate(lexeme)) {
        return Token(TokenType::DATE, lexeme, startLine, startColumn);
    }

    if (isValidTime(lexeme)) {
        return Token(TokenType::TIME, lexeme, startLine, startColumn);
    }

    bool onlyDigits = true;
    for (char c : lexeme) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            onlyDigits = false;
            break;
        }
    }

    if (onlyDigits) {
        return Token(TokenType::INTEGER, lexeme, startLine, startColumn);
    }

    errorManager.addError(LexicalError(
        lexeme,
        "Numero/fecha/hora invalida",
        "El valor no corresponde a un entero, fecha valida o hora valida.",
        startLine,
        startColumn,
        "ERROR"
    ));

    return Token(TokenType::UNKNOWN, lexeme, startLine, startColumn);
}

Token LexicalAnalyzer::readString() {
    int startLine = line;
    int startColumn = column;
    std::string lexeme;
    std::string innerText;

    lexeme += currentChar();
    advance();

    while (currentChar() != '"' && currentChar() != '\0' && currentChar() != '\n') {
        innerText += currentChar();
        lexeme += currentChar();
        advance();
    }

    if (currentChar() == '"') {
        lexeme += currentChar();
        advance();

        if (isBloodType(innerText)) {
            return Token(TokenType::BLOOD_TYPE, lexeme, startLine, startColumn);
        }

        if (isCodeId(innerText)) {
            return Token(TokenType::CODE_ID, lexeme, startLine, startColumn);
        }

return Token(TokenType::STRING, lexeme, startLine, startColumn);
    }

    errorManager.addError(LexicalError(
        lexeme,
        "Cadena sin cerrar",
        "Se encontro una cadena sin comilla de cierre.",
        startLine,
        startColumn,
        "CRITICO"
    ));

    return Token(TokenType::UNKNOWN, lexeme, startLine, startColumn);
}

Token LexicalAnalyzer::makeUnknownToken() {
    int startLine = line;
    int startColumn = column;
    std::string lexeme(1, currentChar());

    errorManager.addError(LexicalError(
        lexeme,
        "Caracter ilegal",
        "Se encontro un caracter no reconocido por el lenguaje.",
        startLine,
        startColumn,
        "ERROR"
    ));

    advance();
    return Token(TokenType::UNKNOWN, lexeme, startLine, startColumn);
}
