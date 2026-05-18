#pragma once
#include <memory>
#include <any>
#include "Token.hpp"

// Forward declarations
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable; // NEW
struct Assign;   // NEW
struct Input;

struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual std::any visitBinaryExpr(const Binary& expr) = 0;
    virtual std::any visitGroupingExpr(const Grouping& expr) = 0;
    virtual std::any visitLiteralExpr(const Literal& expr) = 0;
    virtual std::any visitUnaryExpr(const Unary& expr) = 0;
    virtual std::any visitVariableExpr(const Variable& expr) = 0; // NEW
    virtual std::any visitAssignExpr(const Assign& expr) = 0;     // NEW
    virtual std::any visitInputExpr(const Input& expr) = 0;
};

struct Expr {
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) const = 0;
};

// ... (Binary, Grouping, Literal, and Unary remain exactly the same) ...

struct Binary : public Expr {
    std::unique_ptr<Expr> left; Token op; std::unique_ptr<Expr> right;
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
    std::any accept(ExprVisitor& visitor) const override { return visitor.visitBinaryExpr(*this); }
};

struct Grouping : public Expr {
    std::unique_ptr<Expr> expression;
    Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    std::any accept(ExprVisitor& visitor) const override { return visitor.visitGroupingExpr(*this); }
};

struct Literal : public Expr {
    std::any value;
    Literal(std::any value) : value(std::move(value)) {}
    std::any accept(ExprVisitor& visitor) const override { return visitor.visitLiteralExpr(*this); }
};

struct Unary : public Expr {
    Token op; std::unique_ptr<Expr> right;
    Unary(Token op, std::unique_ptr<Expr> right) : op(std::move(op)), right(std::move(right)) {}
    std::any accept(ExprVisitor& visitor) const override { return visitor.visitUnaryExpr(*this); }
};

// --- NEW NODES FOR VARIABLES ---

struct Variable : public Expr {
    Token name;
    Variable(Token name) : name(std::move(name)) {}
    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitVariableExpr(*this);
    }
};

struct Assign : public Expr {
    Token name;
    std::unique_ptr<Expr> value;
    Assign(Token name, std::unique_ptr<Expr> value) 
        : name(std::move(name)), value(std::move(value)) {}
    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitAssignExpr(*this);
    }
};

struct Input : public Expr {
    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitInputExpr(*this);
    }
};
