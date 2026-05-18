#pragma once
#include "Expr.hpp"
#include "Stmt.hpp"
#include <any>
#include <sstream>
#include <string>

class AstPrinter : public ExprVisitor, public StmtVisitor {
private:
    std::string currentStmtString;

public:
    std::string print(const Expr& expr) {
        return std::any_cast<std::string>(expr.accept(*this));
    }

    std::string print(const Stmt& stmt) {
        currentStmtString = "";
        stmt.accept(*this);
        return currentStmtString;
    }

    std::any visitBinaryExpr(const Binary& expr) override {
        return parenthesize(expr.op.lexeme, *expr.left, *expr.right);
    }

    std::any visitGroupingExpr(const Grouping& expr) override {
        return parenthesize("group", *expr.expression);
    }

    std::any visitLiteralExpr(const Literal& expr) override {
        if (!expr.value.has_value()) return std::string("nil");

        if (expr.value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(expr.value);
        }
        if (expr.value.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(expr.value));
        }
        if (expr.value.type() == typeid(bool)) {
            return std::any_cast<bool>(expr.value) ? std::string("true") : std::string("false");
        }
        return std::string("unknown");
    }

    std::any visitUnaryExpr(const Unary& expr) override {
        return parenthesize(expr.op.lexeme, *expr.right);
    }

    std::any visitVariableExpr(const Variable& expr) override {
        return expr.name.lexeme;
    }

    std::any visitAssignExpr(const Assign& expr) override {
        return parenthesize("= " + expr.name.lexeme, *expr.value);
    }

    std::any visitInputExpr(const Input& expr) override {
        return std::string("input");
    }

    void visitExpressionStmt(const ExpressionStmt& stmt) override {
        currentStmtString = parenthesize("expr-stmt", *stmt.expression);
    }

    void visitPrintStmt(const PrintStmt& stmt) override {
        currentStmtString = parenthesize("print", *stmt.expression);
    }

    void visitVarStmt(const VarStmt& stmt) override {
        if (stmt.initializer) {
            currentStmtString = "(let " + stmt.name.lexeme + " = " + print(*stmt.initializer) + ")";
        } else {
            currentStmtString = "(let " + stmt.name.lexeme + ")";
        }
    }

    void visitBlockStmt(const BlockStmt& stmt) override {
        std::string result = "(block";
        for (const auto& statement : stmt.statements) {
            if (statement) result += " " + print(*statement);
        }
        result += ")";
        currentStmtString = result;
    }

    void visitIfStmt(const IfStmt& stmt) override {
        std::string result = "(if " + print(*stmt.condition) + " ";
        stmt.thenBranch->accept(*this);
        result += currentStmtString;
        if (stmt.elseBranch) {
            stmt.elseBranch->accept(*this);
            result += " else " + currentStmtString;
        }
        result += ")";
        currentStmtString = result;
    }

    void visitWhileStmt(const WhileStmt& stmt) override {
        std::string result = "(while " + print(*stmt.condition) + " ";
        stmt.body->accept(*this);
        result += currentStmtString + ")";
        currentStmtString = result;
    }

private:
    std::string parenthesize(const std::string& name, const Expr& expr) {
        return "(" + name + " " + std::any_cast<std::string>(expr.accept(*this)) + ")";
    }

    std::string parenthesize(const std::string& name, const Expr& expr1, const Expr& expr2) {
        return "(" + name + " " +
               std::any_cast<std::string>(expr1.accept(*this)) + " " +
               std::any_cast<std::string>(expr2.accept(*this)) + ")";
    }
};
