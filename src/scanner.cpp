#include "Scanner.hpp"

// Constructor implementation
Scanner::Scanner(const std::string& source) : source(source) {
    // In C++, std::vector is automatically initialized as empty.
    // We do not need the equivalent of `= new ArrayList<>()`.
}

// A temporary stub so main.cpp can compile without crashing
std::vector<Token> Scanner::scanTokens() {
    return tokens;
}