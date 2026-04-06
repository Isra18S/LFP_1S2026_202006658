#include "ErrorManager.h"

void ErrorManager::addError(const LexicalError& error) {
    errors.push_back(error);
}

const std::vector<LexicalError>& ErrorManager::getErrors() const {
    return errors;
}

bool ErrorManager::hasErrors() const {
    return !errors.empty();
}

void ErrorManager::clear() {
    errors.clear();
}
