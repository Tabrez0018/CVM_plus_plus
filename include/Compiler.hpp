#pragma once
#include "Expr.hpp"
#include "Stmt.hpp" // NEW
#include "chunk.hpp"
#include <vector>   // NEW
#include <iostream>
#include <iomanip>

// Notice multiple inheritance here:
class Compiler : public ExprVisitor, public StmtVisitor {
private:
    Chunk currentChunk;

    void emitByte(uint8_t byte);
    void emitByte(OpCode code);
    void emitBytes(OpCode byte1, OpCode byte2);

public:
    // CHANGED: Now takes a vector of statements instead of one expression
    Chunk compile(const std::vector<std::unique_ptr<Stmt>>& statements);

    // --- EXPR VISITOR OVERRIDES ---
    std::any visitBinaryExpr(const Binary& expr) override;
    std::any visitGroupingExpr(const Grouping& expr) override;
    std::any visitLiteralExpr(const Literal& expr) override;
    std::any visitUnaryExpr(const Unary& expr) override;
    std::any visitVariableExpr(const Variable& expr) override; // NEW
    std::any visitAssignExpr(const Assign& expr) override;     // NEW

    // --- STMT VISITOR OVERRIDES ---
    void visitExpressionStmt(const ExpressionStmt& stmt) override;
    void visitPrintStmt(const PrintStmt& stmt) override;
    void visitVarStmt(const VarStmt& stmt) override;

    void disassembleChunk(const std::string& name);
};