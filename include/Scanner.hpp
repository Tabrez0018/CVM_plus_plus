#pragma once
#include <string>
#include <vector>
#include <unordered_map> // Good call adding this for later!
#include "Token.hpp"

class Scanner {
private:
    std::string source;
    std::vector<Token> tokens;

    // --- ADD THESE MISSING STATE VARIABLES ---
    int start = 0;
    int current = 0;
    int line = 1;

    // --- ADD THESE MISSING METHOD DECLARATIONS ---
    bool isAtEnd() const;
    void scanToken();
    char advance();                     // <-- Add this!
    void addToken(TokenType type);      // <-- Add this!
    bool match(char expected);
    char peek() const;
    void string();
    bool isDigit(char c) const;
    char peekNext() const;
    void number();
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    void identifier();


    std::unordered_map<std::string, TokenType> keywords = {
        {"and",    TokenType::AND},
        {"else",   TokenType::ELSE},
        {"false",  TokenType::FALSE},
        {"for",    TokenType::FOR},
        {"if",     TokenType::IF},
        {"input",  TokenType::INPUT},
        {"let",    TokenType::LET},
        {"or",     TokenType::OR},
        {"print",  TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"true",   TokenType::TRUE},
        {"while",  TokenType::WHILE}
    };

public:
    // Constructor
    Scanner(const std::string& source);

    // We know from main.cpp that we will need this function!
    std::vector<Token> scanTokens();
};