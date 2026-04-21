#pragma once
#include <memory>
#include <any>
#include "Token.hpp"

// Forward declarations of our expression classes so the Visitor knows they exist
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

// ------------------------------------------------------------------------
// THE VISITOR INTERFACE
// This allows us to add new operations (like printing or evaluating) 
// to our expressions without modifying the expression classes themselves.
// ------------------------------------------------------------------------
struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual std::any visitBinaryExpr(const Binary& expr) = 0;
    virtual std::any visitGroupingExpr(const Grouping& expr) = 0;
    virtual std::any visitLiteralExpr(const Literal& expr) = 0;
    virtual std::any visitUnaryExpr(const Unary& expr) = 0;
};

// ------------------------------------------------------------------------
// THE BASE EXPRESSION CLASS
// ------------------------------------------------------------------------
struct Expr {
    virtual ~Expr() = default;
    
    // The core of the Visitor pattern. Every derived expression must implement this.
    virtual std::any accept(ExprVisitor& visitor) const = 0;
};

// ------------------------------------------------------------------------
// AST NODE IMPLEMENTATIONS
// ------------------------------------------------------------------------

struct Binary : public Expr {
    // We use unique_ptr because a Binary expression OWNS its left and right children
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitBinaryExpr(*this);
    }
};

struct Grouping : public Expr {
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitGroupingExpr(*this);
    }
};

struct Literal : public Expr {
    // std::any can hold an int, a double, a bool, or a string!
    std::any value;

    Literal(std::any value) : value(std::move(value)) {}

    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitLiteralExpr(*this);
    }
};

struct Unary : public Expr {
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}

    std::any accept(ExprVisitor& visitor) const override {
        return visitor.visitUnaryExpr(*this);
    }
};