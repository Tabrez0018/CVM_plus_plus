#pragma once
#include <vector>
#include <cstdint>
#include <any>

enum class OpCode : uint8_t {
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_TRUE,
    OP_FALSE,
    
    // Variables
    OP_DEFINE_GLOBAL, 
    OP_GET_GLOBAL,    
    OP_SET_GLOBAL,    
    
    // Statements
    OP_PRINT,
    OP_POP,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_INPUT,

    OP_RETURN
};

struct Chunk {
    std::vector<uint8_t> code;
    std::vector<std::any> constants;
    
    void write(uint8_t byte) {
        code.push_back(byte);
    }
    
    void write(OpCode codeByte) {
        write(static_cast<uint8_t>(codeByte));
    }

    int addConstant(std::any value) {
        constants.push_back(std::move(value));
        return constants.size() - 1;
    }
};
