cvm-plus-plus/
├── src/
│   ├── main.cpp        # Deliverable: CLI-based REPL / File Runner demo
│   ├── token.cpp       # Part of Lexer module
│   ├── scanner.cpp     # Deliverable: Lexer module (Completed!)
│   ├── parser.cpp      # Deliverable: Parser module (Arranges Tokens into AST)
│   ├── compiler.cpp    # Deliverable: Bytecode Compiler module (Flattens AST)
│   └── vm.cpp          # Deliverable: Virtual Machine execution engine
├── include/
│   ├── Token.h         
│   ├── Scanner.h       
│   ├── Expr.h          # NEW: The AST node definitions for the Parser
│   ├── Parser.h        # NEW: Blueprint for parser.cpp
│   ├── Compiler.h      # NEW: Blueprint for compiler.cpp
│   ├── common.h        
│   └── chunk.h         
├── tests/
│   ├── simple_math.cvm 
│   └── loops.cvm       
└── CMakeLists.txt