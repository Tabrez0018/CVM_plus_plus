#include "Token.hpp"

// 1. Constructor Implementation
// We use a "member initializer list" (the colon syntax) because it is 
// the most efficient way to assign variables when a C++ object is created.
Token::Token(TokenType type, std::string lexeme, int line) 
    : type(type), lexeme(lexeme), line(line) {}

// 2. toString() Implementation
// The 'const' keyword guarantees this method won't accidentally change the token's data
std::string Token::toString() const {
    // Enums in C++ don't automatically print their names (like "LEFT_PAREN").
    // For now, we cast the enum to an int just to see its numeric ID in our debug prints.
    return "Type: " + std::to_string(static_cast<int>(type)) + 
           " | Lexeme: '" + lexeme + 
           "' | Line: " + std::to_string(line);
}