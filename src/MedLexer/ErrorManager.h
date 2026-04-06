#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <vector>
#include "LexicalError.h"

class ErrorManager {
private:
    std::vector<LexicalError> errors;

public:
    void addError(const LexicalError& error);
    const std::vector<LexicalError>& getErrors() const;
    bool hasErrors() const;
    void clear();
};

#endif
