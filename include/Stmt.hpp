#pragma once
#include "Expr.hpp"
#include <memory>
#include <vector>

struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct BlockStmt;

// The Visitor for Statements
struct StmtVisitor {
    virtual ~StmtVisitor() = default;
    virtual void visitExpressionStmt(const ExpressionStmt& stmt) = 0;
    virtual void visitPrintStmt(const PrintStmt& stmt) = 0;
    virtual void visitVarStmt(const VarStmt& stmt) = 0;
    virtual void visitBlockStmt(const BlockStmt& stmt) = 0;
};

// Base Statement Class
struct Stmt {
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) const = 0;
};

// A standard line of math ending in a semicolon (e.g., "1 + 2;")
struct ExpressionStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    void accept(StmtVisitor& visitor) const override { visitor.visitExpressionStmt(*this); }
};

// A print statement (e.g., "print x;")
struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    void accept(StmtVisitor& visitor) const override { visitor.visitPrintStmt(*this); }
};

// A variable declaration (e.g., "let x = 10;")
struct VarStmt : public Stmt {
    Token name;
    std::unique_ptr<Expr> initializer; // This can be null if they just type "let x;"
    VarStmt(Token name, std::unique_ptr<Expr> initializer) 
        : name(std::move(name)), initializer(std::move(initializer)) {}
    void accept(StmtVisitor& visitor) const override { visitor.visitVarStmt(*this); }
};

struct BlockStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
    void accept(StmtVisitor& visitor) const override { visitor.visitBlockStmt(*this); }
};