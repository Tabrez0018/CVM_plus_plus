#pragma once
#include <vector>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include "Token.hpp"
#include "Expr.hpp"
#include "Stmt.hpp" // <-- NEW

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

    // --- NEW STATEMENT RULES ---
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::vector<std::unique_ptr<Stmt>> block();
    
    // --- ERROR RECOVERY ---
    void synchronize(); // Helps the parser recover if it hits a bad line

    // --- EXISTING EXPRESSION RULES ---
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    // ... (Keep equality, comparison, term, factor, unary, primary exactly the same)
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

    class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& message) : std::runtime_error(message) {}
    };
    ParseError error(const Token& token, const std::string& message);

public:
    Parser(const std::vector<Token>& tokens);
    
    // CHANGED: Now returns a list of statements instead of one expression!
    std::vector<std::unique_ptr<Stmt>> parse(); 
};