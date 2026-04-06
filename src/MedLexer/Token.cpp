#include "Token.h"

Token::Token() : type(TokenType::UNKNOWN), lexeme(""), line(1), column(1) {}

Token::Token(TokenType type, const std::string& lexeme, int line, int column)
    : type(type), lexeme(lexeme), line(line), column(column) {}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::SECTION: return "SECTION";
        case TokenType::ELEMENT: return "ELEMENT";
        case TokenType::ATTRIBUTE: return "ATTRIBUTE";
        case TokenType::STRING: return "STRING";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::DATE: return "DATE";
        case TokenType::TIME: return "TIME";
        case TokenType::CODE_ID: return "CODE_ID";
        case TokenType::BLOOD_TYPE: return "BLOOD_TYPE";
        case TokenType::SPECIALTY: return "SPECIALTY";
        case TokenType::DOSE: return "DOSE";
        case TokenType::DELIMITER: return "DELIMITER";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "UNKNOWN";
    }
}
