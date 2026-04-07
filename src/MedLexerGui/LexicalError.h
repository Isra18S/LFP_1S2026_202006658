#ifndef LEXICALERROR_H
#define LEXICALERROR_H

#include <string>

class LexicalError {
public:
    std::string lexeme;
    std::string type;
    std::string description;
    int line;
    int column;
    std::string severity;

    LexicalError();
    LexicalError(
        const std::string& lexeme,
        const std::string& type,
        const std::string& description,
        int line,
        int column,
        const std::string& severity
    );
};

#endif
