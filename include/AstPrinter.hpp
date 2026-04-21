#pragma once
#include "Expr.hpp"
#include <string>
#include <sstream>

class AstPrinter : public ExprVisitor {
public:
    // This is the public API we call to kick off the printing process
    std::string print(const Expr& expr) {
        // expr.accept returns std::any, so we cast it back to a string
        return std::any_cast<std::string>(expr.accept(*this));
    }

    // --- VISITOR OVERRIDES ---

    std::any visitBinaryExpr(const Binary& expr) override {
        return parenthesize(expr.op.lexeme, *expr.left, *expr.right);
    }

    std::any visitGroupingExpr(const Grouping& expr) override {
        return parenthesize("group", *expr.expression);
    }

    std::any visitLiteralExpr(const Literal& expr) override {
        if (!expr.value.has_value()) return std::string("nil");
        
        // We must check what type is actually inside the std::any
        if (expr.value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(expr.value);
        } 
        else if (expr.value.type() == typeid(double)) {
            // std::to_string converts numbers to strings
            return std::to_string(std::any_cast<double>(expr.value));
        } 
        else if (expr.value.type() == typeid(bool)) {
            return std::any_cast<bool>(expr.value) ? std::string("true") : std::string("false");
        }
        
        return std::string("unknown");
    }

    std::any visitUnaryExpr(const Unary& expr) override {
        return parenthesize(expr.op.lexeme, *expr.right);
    }

private:
    // --- HELPER METHODS ---
    // These format the nodes into Lisp-like strings: (+ 1 2)

    std::string parenthesize(const std::string& name, const Expr& expr) {
        return "(" + name + " " + std::any_cast<std::string>(expr.accept(*this)) + ")";
    }

    std::string parenthesize(const std::string& name, const Expr& expr1, const Expr& expr2) {
        return "(" + name + " " + 
               std::any_cast<std::string>(expr1.accept(*this)) + " " + 
               std::any_cast<std::string>(expr2.accept(*this)) + ")";
    }
};