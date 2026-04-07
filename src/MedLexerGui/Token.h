#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class MedTokenType {
    SECTION,
    ELEMENT,
    ATTRIBUTE,
    STRING,
    INTEGER,
    DATE,
    TIME,
    CODE_ID,
    BLOOD_TYPE,
    SPECIALTY,
    DOSE,
    DELIMITER,
    END_OF_FILE,
    UNKNOWN
};

class Token {
public:
    MedTokenType type;
    std::string lexeme;
    int line;
    int column;

    Token();
    Token(MedTokenType type, const std::string& lexeme, int line, int column);
};

std::string tokenTypeToString(MedTokenType type);

#endif
