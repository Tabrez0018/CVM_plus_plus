#include "vm.hpp"
#include <iostream>
#include <sstream>

VM::VM() : chunk(nullptr), ip(0) {}

void VM::push(std::any value) {
    stack.push_back(std::move(value));
}

std::any VM::pop() {
    std::any value = std::move(stack.back());
    stack.pop_back();
    return value;
}

std::any VM::peek(int distance) const {
    return stack[stack.size() - 1 - distance];
}

bool VM::isFalsey(const std::any& value) const {
    if (!value.has_value()) return true;
    if (value.type() == typeid(bool)) return !std::any_cast<bool>(value);
    if (value.type() == typeid(double)) return std::any_cast<double>(value) == 0;
    if (value.type() == typeid(std::string)) return std::any_cast<std::string>(value).empty();
    return false;
}

void VM::printValue(const std::any& value) const {
    if (value.type() == typeid(double)) std::cout << std::any_cast<double>(value);
    else if (value.type() == typeid(bool)) std::cout << (std::any_cast<bool>(value) ? "true" : "false");
    else if (value.type() == typeid(std::string)) std::cout << std::any_cast<std::string>(value);
}

InterpretResult VM::interpret(Chunk* chunk) {
    this->chunk = chunk;
    this->ip = 0;
    return run();
}

InterpretResult VM::run() {
    #define READ_BYTE() (chunk->code[ip++])
    #define READ_CONSTANT() (chunk->constants[READ_BYTE()])
    #define READ_SHORT() (ip += 2, static_cast<uint16_t>((chunk->code[ip - 2] << 8) | chunk->code[ip - 1]))
    #define BINARY_OP(op) \
        do { \
            double b = std::any_cast<double>(pop()); \
            double a = std::any_cast<double>(pop()); \
            push(a op b); \
        } while (false)

    for (;;) {
        uint8_t instruction = READ_BYTE();

        switch (static_cast<OpCode>(instruction)) {
            case OpCode::OP_CONSTANT: {
                std::any constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OpCode::OP_DEFINE_GLOBAL: {
                std::string name = std::any_cast<std::string>(READ_CONSTANT());
                globals[name] = pop();
                break;
            }
            case OpCode::OP_GET_GLOBAL: {
                std::string name = std::any_cast<std::string>(READ_CONSTANT());
                auto match = globals.find(name);
                if (match != globals.end()) {
                    push(match->second);
                } else {
                    std::cerr << "Runtime Error: Undefined variable '" << name << "'.\n";
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OpCode::OP_SET_GLOBAL: {
                std::string name = std::any_cast<std::string>(READ_CONSTANT());
                auto match = globals.find(name);
                if (match != globals.end()) {
                    globals[name] = peek(0);
                } else {
                    std::cerr << "Runtime Error: Undefined variable '" << name << "'.\n";
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OpCode::OP_PRINT: {
                std::any value = pop();
                printValue(value);
                std::cout << "\n";
                break;
            }
            case OpCode::OP_POP: {
                pop();
                break;
            }
            case OpCode::OP_ADD:      BINARY_OP(+); break;
            case OpCode::OP_SUBTRACT: BINARY_OP(-); break;
            case OpCode::OP_MULTIPLY: BINARY_OP(*); break;
            case OpCode::OP_DIVIDE:   BINARY_OP(/); break;
            case OpCode::OP_GREATER:  BINARY_OP(>); break;
            case OpCode::OP_LESS:     BINARY_OP(<); break;

            case OpCode::OP_NEGATE: {
                push(-std::any_cast<double>(pop()));
                break;
            }
            case OpCode::OP_NOT: {
                push(isFalsey(pop()));
                break;
            }
            case OpCode::OP_EQUAL: {
                std::any b = pop();
                std::any a = pop();
                if (a.type() == typeid(double) && b.type() == typeid(double)) {
                    push(std::any_cast<double>(a) == std::any_cast<double>(b));
                } else if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
                    push(std::any_cast<bool>(a) == std::any_cast<bool>(b));
                } else {
                    push(false);
                }
                break;
            }
            case OpCode::OP_TRUE:  push(true); break;
            case OpCode::OP_FALSE: push(false); break;

            case OpCode::OP_INPUT: {
                std::string text;
                std::getline(std::cin, text);

                std::stringstream parser(text);
                double number;
                char leftover;
                if (parser >> number && !(parser >> leftover)) {
                    push(number);
                } else {
                    push(text);
                }
                break;
            }

            case OpCode::OP_JUMP: {
                uint16_t offset = READ_SHORT();
                ip += offset;
                break;
            }

            case OpCode::OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(peek(0))) ip += offset;
                break;
            }

            case OpCode::OP_LOOP: {
                uint16_t offset = READ_SHORT();
                ip -= offset;
                break;
            }

            case OpCode::OP_RETURN: {
                return InterpretResult::INTERPRET_OK;
            }
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef READ_SHORT
    #undef BINARY_OP
}
