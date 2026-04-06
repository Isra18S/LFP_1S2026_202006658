#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "LexicalAnalyzer.h"

int main() {
    std::ifstream file("../../examples/ejemplo1.med");
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo de prueba." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    LexicalAnalyzer analyzer(content);
    std::vector<Token> tokens = analyzer.analyze();

    std::cout << "=== TOKENS ===" << std::endl;
    for (size_t i = 0; i < tokens.size(); i++) {
        std::cout << i + 1 << ". "
                  << tokens[i].lexeme << " | "
                  << tokenTypeToString(tokens[i].type) << " | "
                  << "Linea: " << tokens[i].line
                  << ", Columna: " << tokens[i].column
                  << std::endl;
    }

    std::cout << "\n=== ERRORES ===" << std::endl;
    const std::vector<LexicalError>& errors = analyzer.getErrors();
    for (size_t i = 0; i < errors.size(); i++) {
        std::cout << i + 1 << ". "
                  << errors[i].lexeme << " | "
                  << errors[i].type << " | "
                  << errors[i].description << " | "
                  << "Linea: " << errors[i].line
                  << ", Columna: " << errors[i].column
                  << " | Gravedad: " << errors[i].severity
                  << std::endl;
    }

    return 0;
}
