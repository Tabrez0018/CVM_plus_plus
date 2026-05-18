# CVM++ Project Report

## Project Title

Stack-Based Virtual Machine and Custom Compiler

## Objective

The goal of this project is to understand how raw source code is translated into executable instructions by building a small scripting language in C++. The language is scanned into tokens, parsed into an Abstract Syntax Tree, compiled into custom bytecode, and executed on a stack-based Virtual Machine.

The project follows the architecture taught in Crafting Interpreters by Robert Nystrom, adapted into C++ using classes, headers, `std::unique_ptr`, `std::vector`, and `std::any`.

## Implemented Compiler Pipeline

### 1. Lexer / Scanner

Files:

- `include/Scanner.hpp`
- `src/scanner.cpp`
- `include/Token.hpp`
- `src/token.cpp`

The scanner reads raw source code and converts it into tokens. It supports numbers, strings, identifiers, keywords, comments, operators, braces, parentheses, and semicolons.

Example:

```c
let x = 10;
print x + 5;
```

This becomes a stream of tokens such as `LET`, `IDENTIFIER`, `EQUAL`, `NUMBER`, `SEMICOLON`, `PRINT`, and so on.

### 2. Parser and AST

Files:

- `include/Expr.hpp`
- `include/Stmt.hpp`
- `include/Parser.hpp`
- `src/parser.cpp`

The parser uses recursive descent parsing. It arranges tokens into an Abstract Syntax Tree. The AST has expression nodes such as binary expressions, unary expressions, literals, grouping, variables, assignment, and input. It also has statement nodes for print statements, variable declarations, blocks, if statements, and while loops.

### 3. Bytecode Compiler

Files:

- `include/Compiler.hpp`
- `src/compiler.cpp`
- `include/chunk.hpp`

The compiler walks the AST using the visitor pattern and emits bytecode instructions into a `Chunk`. Constants are stored in a constants table, while instructions are stored as bytes.

Important opcodes include:

- `OP_CONSTANT`
- `OP_ADD`
- `OP_SUBTRACT`
- `OP_MULTIPLY`
- `OP_DIVIDE`
- `OP_EQUAL`
- `OP_GREATER`
- `OP_LESS`
- `OP_DEFINE_GLOBAL`
- `OP_GET_GLOBAL`
- `OP_SET_GLOBAL`
- `OP_PRINT`
- `OP_JUMP`
- `OP_JUMP_IF_FALSE`
- `OP_LOOP`
- `OP_INPUT`
- `OP_RETURN`

### 4. Stack-Based Virtual Machine

Files:

- `include/vm.hpp`
- `src/vm.cpp`

The VM executes bytecode using a stack. Arithmetic operations pop operands from the stack and push results back. Variables are stored in a global variable table. Jump instructions are used to implement `if/else` and `while` loops.

### 5. CLI, REPL, and File Runner

File:

- `src/main.cpp`

The program can run in two modes:

- REPL mode when no script file is passed.
- File-runner mode when a `.cvm` script is passed.

Debug mode prints the generated AST and bytecode before executing the VM output.

## Language Features

The current language supports:

- Numbers
- Booleans
- Strings
- Arithmetic: `+`, `-`, `*`, `/`
- Comparisons: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Variables with `let`
- Assignment
- Blocks using `{ ... }`
- `print`
- `input`
- `if/else`
- `while` loops
- REPL execution
- File execution
- AST debug output
- Bytecode debug output

## Sample Program

```c
let a = 10;
let b = 5;

print "Calculator demo:";
print a + b;
print a - b;
print a * b;
print a / b;
```

The compiler prints the AST, then prints bytecode instructions, then the VM runs the bytecode and prints the final results.

## Limitations

- Variables are global. Block-local scope is not implemented.
- Numbers are represented as `double`, even though the project scope mentions integers.
- Runtime type checking is basic.
- Functions and arrays are not implemented.

## Conclusion

CVM++ demonstrates the major stages of a compiler and virtual machine: lexical analysis, parsing, AST construction, bytecode generation, and stack-based bytecode execution. The project meets the core requirements of the problem statement while keeping the language small and understandable.
