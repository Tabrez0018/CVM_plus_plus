#pragma once
#include <vector>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include "Token.hpp"
#include "Expr.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

    // --- GRAMMAR RULES ---
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    // --- HELPER METHODS ---
    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    Token advance();
    Token peek() const;
    Token previous() const;
    Token consume(TokenType type, const std::string& message);

    // Error handling
    class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& message) : std::runtime_error(message) {}
    };
    ParseError error(const Token& token, const std::string& message);

public:
    Parser(const std::vector<Token>& tokens);
    
    // The main entry point
    std::unique_ptr<Expr> parse();
};