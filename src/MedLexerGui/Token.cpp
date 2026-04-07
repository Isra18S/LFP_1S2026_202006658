#include "Token.h"

Token::Token()
    : type(MedTokenType::UNKNOWN), lexeme(""), line(1), column(1) {
}

Token::Token(MedTokenType type, const std::string& lexeme, int line, int column)
    : type(type), lexeme(lexeme), line(line), column(column) {
}

std::string tokenTypeToString(MedTokenType type) {
    switch (type) {
        case MedTokenType::SECTION: return "SECTION";
        case MedTokenType::ELEMENT: return "ELEMENT";
        case MedTokenType::ATTRIBUTE: return "ATTRIBUTE";
        case MedTokenType::STRING: return "STRING";
        case MedTokenType::INTEGER: return "INTEGER";
        case MedTokenType::DATE: return "DATE";
        case MedTokenType::TIME: return "TIME";
        case MedTokenType::CODE_ID: return "CODE_ID";
        case MedTokenType::BLOOD_TYPE: return "BLOOD_TYPE";
        case MedTokenType::SPECIALTY: return "SPECIALTY";
        case MedTokenType::DOSE: return "DOSE";
        case MedTokenType::DELIMITER: return "DELIMITER";
        case MedTokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "UNKNOWN";
    }
}
