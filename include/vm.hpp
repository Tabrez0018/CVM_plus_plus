#pragma once
#include "chunk.hpp"
#include <vector>
#include <any>
#include <unordered_map>
#include <string>

enum class InterpretResult {
    INTERPRET_OK,
    INTERPRET_RUNTIME_ERROR
};

class VM {
private:
    Chunk* chunk;
    size_t ip; 
    std::vector<std::any> stack;

    // --- NEW: The global variable memory ---
    std::unordered_map<std::string, std::any> globals;

    void push(std::any value);
    std::any pop();
    std::any peek(int distance) const; // NEW: Look at the stack without popping

    InterpretResult run();

public:
    VM();
    InterpretResult interpret(Chunk* chunk);
};