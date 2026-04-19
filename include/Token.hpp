#pragma once
#include <string>

// The vocabulary of CVM++
enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, 
    LEFT_BRACE, RIGHT_BRACE,
    MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, NUMBER,

    // Keywords.
    ELSE, FALSE, IF, PRINT, TRUE, WHILE, LET, INPUT, FOR, AND, OR, RETURN, 

    EOF_TOKEN
};

// The Token class to hold our parsed lexemes
class Token {
public:
    TokenType type;
    std::string lexeme;
    int line;

    // Constructor
    Token(TokenType type, std::string lexeme, int line);

    // Replaces Java's toString()
    std::string toString() const;
};
