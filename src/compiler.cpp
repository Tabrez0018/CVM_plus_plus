#include "Compiler.hpp"

void Compiler::emitByte(uint8_t byte) { currentChunk.write(byte); }
void Compiler::emitByte(OpCode code) { currentChunk.write(code); }
void Compiler::emitBytes(OpCode byte1, OpCode byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

int Compiler::emitJump(OpCode instruction) {
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return static_cast<int>(currentChunk.code.size()) - 2;
}

void Compiler::emitLoop(int loopStart) {
    emitByte(OpCode::OP_LOOP);

    int offset = static_cast<int>(currentChunk.code.size()) - loopStart + 2;
    emitByte(static_cast<uint8_t>((offset >> 8) & 0xff));
    emitByte(static_cast<uint8_t>(offset & 0xff));
}

void Compiler::patchJump(int offset) {
    int jump = static_cast<int>(currentChunk.code.size()) - offset - 2;
    currentChunk.code[offset] = static_cast<uint8_t>((jump >> 8) & 0xff);
    currentChunk.code[offset + 1] = static_cast<uint8_t>(jump & 0xff);
}

Chunk Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& statements) {
    for (const auto& statement : statements) {
        if (statement) statement->accept(*this);
    }
    emitByte(OpCode::OP_RETURN); 
    return currentChunk;
}

std::any Compiler::visitLiteralExpr(const Literal& expr) {
    if (expr.value.type() == typeid(bool)) {
        bool val = std::any_cast<bool>(expr.value);
        emitByte(val ? OpCode::OP_TRUE : OpCode::OP_FALSE);
    } else {
        int index = currentChunk.addConstant(expr.value);
        emitByte(OpCode::OP_CONSTANT);
        emitByte(static_cast<uint8_t>(index));
    }
    return {}; 
}

std::any Compiler::visitGroupingExpr(const Grouping& expr) {
    expr.expression->accept(*this);
    return {};
}

std::any Compiler::visitUnaryExpr(const Unary& expr) {
    expr.right->accept(*this);
    if (expr.op.type == TokenType::MINUS) emitByte(OpCode::OP_NEGATE);
    else if (expr.op.type == TokenType::BANG) emitByte(OpCode::OP_NOT);
    return {};
}

std::any Compiler::visitBinaryExpr(const Binary& expr) {
    expr.left->accept(*this);
    expr.right->accept(*this);
    switch (expr.op.type) {
        case TokenType::PLUS:          emitByte(OpCode::OP_ADD); break;
        case TokenType::MINUS:         emitByte(OpCode::OP_SUBTRACT); break;
        case TokenType::STAR:          emitByte(OpCode::OP_MULTIPLY); break;
        case TokenType::SLASH:         emitByte(OpCode::OP_DIVIDE); break;
        case TokenType::EQUAL_EQUAL:   emitByte(OpCode::OP_EQUAL); break;
        case TokenType::GREATER:       emitByte(OpCode::OP_GREATER); break;
        case TokenType::LESS:          emitByte(OpCode::OP_LESS); break;
        case TokenType::BANG_EQUAL:    emitBytes(OpCode::OP_EQUAL, OpCode::OP_NOT); break;
        case TokenType::LESS_EQUAL:    emitBytes(OpCode::OP_GREATER, OpCode::OP_NOT); break;
        case TokenType::GREATER_EQUAL: emitBytes(OpCode::OP_LESS, OpCode::OP_NOT); break;
        default: break;
    }
    return {};
}

std::any Compiler::visitVariableExpr(const Variable& expr) {
    int index = currentChunk.addConstant(expr.name.lexeme);
    emitBytes(OpCode::OP_GET_GLOBAL, static_cast<OpCode>(index));
    return {};
}

std::any Compiler::visitAssignExpr(const Assign& expr) {
    expr.value->accept(*this); 
    int index = currentChunk.addConstant(expr.name.lexeme);
    emitBytes(OpCode::OP_SET_GLOBAL, static_cast<OpCode>(index));
    return {};
}

std::any Compiler::visitInputExpr(const Input& expr) {
    emitByte(OpCode::OP_INPUT);
    return {};
}

void Compiler::visitExpressionStmt(const ExpressionStmt& stmt) {
    stmt.expression->accept(*this);
    emitByte(OpCode::OP_POP); 
}

void Compiler::visitPrintStmt(const PrintStmt& stmt) {
    stmt.expression->accept(*this);
    emitByte(OpCode::OP_PRINT);
}

void Compiler::visitVarStmt(const VarStmt& stmt) {
    if (stmt.initializer) {
        stmt.initializer->accept(*this); 
    } else {
        emitByte(OpCode::OP_FALSE); 
    }
    int index = currentChunk.addConstant(stmt.name.lexeme);
    emitBytes(OpCode::OP_DEFINE_GLOBAL, static_cast<OpCode>(index));
}

void Compiler::visitBlockStmt(const BlockStmt& stmt) {
    for (const auto& statement : stmt.statements) {
        if (statement) statement->accept(*this);
    }
}

void Compiler::visitIfStmt(const IfStmt& stmt) {
    stmt.condition->accept(*this);

    int thenJump = emitJump(OpCode::OP_JUMP_IF_FALSE);
    emitByte(OpCode::OP_POP);
    stmt.thenBranch->accept(*this);

    int elseJump = emitJump(OpCode::OP_JUMP);

    patchJump(thenJump);
    emitByte(OpCode::OP_POP);

    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }

    patchJump(elseJump);
}

void Compiler::visitWhileStmt(const WhileStmt& stmt) {
    int loopStart = static_cast<int>(currentChunk.code.size());

    stmt.condition->accept(*this);
    int exitJump = emitJump(OpCode::OP_JUMP_IF_FALSE);
    emitByte(OpCode::OP_POP);

    stmt.body->accept(*this);
    emitLoop(loopStart);

    patchJump(exitJump);
    emitByte(OpCode::OP_POP);
}

void Compiler::disassembleChunk(const std::string& name) {
    std::cout << "== " << name << " ==\n";
    for (size_t offset = 0; offset < currentChunk.code.size();) {
        uint8_t instruction = currentChunk.code[offset];
        std::cout << std::setfill('0') << std::setw(4) << offset << " ";

        switch (static_cast<OpCode>(instruction)) {
            case OpCode::OP_CONSTANT: {
                uint8_t constantIndex = currentChunk.code[offset + 1];
                std::cout << "OP_CONSTANT " << (int)constantIndex << " '";
                auto& val = currentChunk.constants[constantIndex];
                if (val.type() == typeid(double)) std::cout << std::any_cast<double>(val);
                else if (val.type() == typeid(std::string)) std::cout << std::any_cast<std::string>(val);
                std::cout << "'\n";
                offset += 2; 
                break;
            }
            case OpCode::OP_DEFINE_GLOBAL: {
                uint8_t index = currentChunk.code[offset + 1];
                std::cout << "OP_DEFINE_GLOBAL " << (int)index << " '" << std::any_cast<std::string>(currentChunk.constants[index]) << "'\n";
                offset += 2; break;
            }
            case OpCode::OP_GET_GLOBAL: {
                uint8_t index = currentChunk.code[offset + 1];
                std::cout << "OP_GET_GLOBAL " << (int)index << " '" << std::any_cast<std::string>(currentChunk.constants[index]) << "'\n";
                offset += 2; break;
            }
            case OpCode::OP_SET_GLOBAL: {
                uint8_t index = currentChunk.code[offset + 1];
                std::cout << "OP_SET_GLOBAL " << (int)index << " '" << std::any_cast<std::string>(currentChunk.constants[index]) << "'\n";
                offset += 2; break;
            }
            case OpCode::OP_PRINT:     std::cout << "OP_PRINT\n"; offset++; break;
            case OpCode::OP_POP:       std::cout << "OP_POP\n"; offset++; break;
            case OpCode::OP_INPUT:     std::cout << "OP_INPUT\n"; offset++; break;
            case OpCode::OP_ADD:       std::cout << "OP_ADD\n"; offset++; break;
            case OpCode::OP_SUBTRACT:  std::cout << "OP_SUBTRACT\n"; offset++; break;
            case OpCode::OP_MULTIPLY:  std::cout << "OP_MULTIPLY\n"; offset++; break;
            case OpCode::OP_DIVIDE:    std::cout << "OP_DIVIDE\n"; offset++; break;
            case OpCode::OP_NEGATE:    std::cout << "OP_NEGATE\n"; offset++; break;
            case OpCode::OP_NOT:       std::cout << "OP_NOT\n"; offset++; break;
            case OpCode::OP_EQUAL:     std::cout << "OP_EQUAL\n"; offset++; break;
            case OpCode::OP_GREATER:   std::cout << "OP_GREATER\n"; offset++; break;
            case OpCode::OP_LESS:      std::cout << "OP_LESS\n"; offset++; break;
            case OpCode::OP_TRUE:      std::cout << "OP_TRUE\n"; offset++; break;
            case OpCode::OP_FALSE:     std::cout << "OP_FALSE\n"; offset++; break;
            case OpCode::OP_JUMP:
            case OpCode::OP_JUMP_IF_FALSE:
            case OpCode::OP_LOOP: {
                uint16_t jump = static_cast<uint16_t>((currentChunk.code[offset + 1] << 8) | currentChunk.code[offset + 2]);
                const char* name =
                    static_cast<OpCode>(instruction) == OpCode::OP_JUMP ? "OP_JUMP" :
                    static_cast<OpCode>(instruction) == OpCode::OP_JUMP_IF_FALSE ? "OP_JUMP_IF_FALSE" :
                    "OP_LOOP";
                std::cout << name << " " << jump << "\n";
                offset += 3;
                break;
            }
            case OpCode::OP_RETURN:    std::cout << "OP_RETURN\n"; offset++; break;
            default:                   std::cout << "Unknown opcode " << (int)instruction << "\n"; offset++; break;
        }
    }
}
