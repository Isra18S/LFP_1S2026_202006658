#include "LexicalError.h"

LexicalError::LexicalError()
    : lexeme(""), type(""), description(""), line(1), column(1), severity("ERROR") {
}

LexicalError::LexicalError(
    const std::string& lexeme,
    const std::string& type,
    const std::string& description,
    int line,
    int column,
    const std::string& severity
)
    : lexeme(lexeme), type(type), description(description),
      line(line), column(column), severity(severity) {
}
