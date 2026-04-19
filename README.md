# CVM++

cvm-plus-plus/
├── src/
│   ├── main.cpp        # The entry point, argument parsing, and REPL loop
│   ├── token.cpp       # Implementation of Token constructor and toString()
│   ├── scanner.cpp     # The state machine that reads text (up next!)
│   ├── parser.cpp      # (Future) Will build the Abstract Syntax Tree (AST)
│   └── vm.cpp          # (Future) The Stack-based Virtual Machine
├── include/
│   ├── Token.h         # TokenType enum and Token class blueprint
│   ├── Scanner.h       # Scanner class blueprint
│   ├── common.h        # (Future) Standard includes and types (like uint8_t)
│   └── chunk.h         # (Future) The dynamic array for our Bytecode
├── tests/
│   ├── simple_math.cvm # Basic arithmetic tests
│   └── loops.cvm       # Testing while/for loops
├── CMakeLists.txt      # (Or Makefile) Build instructions for C++
└── README.md           # Project documentation
