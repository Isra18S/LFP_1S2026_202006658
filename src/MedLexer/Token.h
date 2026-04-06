#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
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
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    Token();
    Token(TokenType type, const std::string& lexeme, int line, int column);
};

std::string tokenTypeToString(TokenType type);

#endif
