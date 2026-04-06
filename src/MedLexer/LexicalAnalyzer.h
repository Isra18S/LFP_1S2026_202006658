#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>
#include "Token.h"
#include "ErrorManager.h"

class LexicalAnalyzer {
private:
    std::string input;
    size_t pos;
    int line;
    int column;
    ErrorManager errorManager;

public:
    LexicalAnalyzer(const std::string& text);

    std::vector<Token> analyze();
    const std::vector<LexicalError>& getErrors() const;

private:
    char currentChar() const;
    char peekChar() const;
    void advance();
    void skipWhitespace();

    Token nextToken();
    Token readWord();
    Token readString();
    Token readNumberOrDateOrTime();
    Token readDelimiter();
    Token makeUnknownToken();

    bool isDelimiter(char c) const;

    bool isSection(const std::string& lexeme) const;
    bool isElement(const std::string& lexeme) const;
    bool isAttribute(const std::string& lexeme) const;
    bool isSpecialty(const std::string& lexeme) const;
    bool isDose(const std::string& lexeme) const;
    bool isBloodType(const std::string& lexeme) const;

    bool isValidDate(const std::string& lexeme) const;
    bool isValidTime(const std::string& lexeme) const;
};

#endif
