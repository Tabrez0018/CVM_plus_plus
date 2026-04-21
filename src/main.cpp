#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

// CVM++ Modules
#include "Token.hpp"
#include "Scanner.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"
#include "Compiler.hpp"
#include "vm.hpp"

// 1. Global State
inline bool hadError = false;

// --- DELIVERABLE: Debug Mode ---
// Set to false when you only want the final output
inline bool DEBUG_MODE = true; 

// 2. Forward Declarations
void runFile(const std::string& path);
void runPrompt();
void run(const std::string& source);
void report(int line, const std::string& where, const std::string& message);
void error(int line, const std::string& message);

// 3. Main Entry Point
int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: cvm++ [script]\n";
        std::exit(64); 
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
    return 0;
}

// 4. Function Definitions
void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << "\n";
        std::exit(74);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());
    
    // Stop the executable if there was a syntax error in the file
    if (hadError) std::exit(65);
}

void runPrompt() {
    std::string line;
    std::cout << "CVM++ Interactive REPL\nType 'exit' to quit.\n";
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line == "exit") {
            std::cout << "\n";
            break;
        }
        run(line);
        hadError = false; // Reset error flag so the REPL doesn't permanently lock up
    }
}

// --- THE CORE PIPELINE ---
void run(const std::string& source) {
    // 1. Lexical Analysis (Scanner)
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // 2. Syntactic Analysis (Parser)
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

    // Stop if there was a syntax error
    if (hadError) return;

    // DELIVERABLE 2: Show the generated AST
    if (DEBUG_MODE) {
        AstPrinter astPrinter;
        std::cout << "\n[DEBUG] --- Abstract Syntax Tree ---\n";
        for (const auto& stmt : statements) {
            if (stmt) std::cout << astPrinter.print(*stmt) << "\n";
        }
    }

    // 3. Bytecode Compilation (Compiler)
    Compiler compiler;
    Chunk chunk = compiler.compile(statements);

    // DELIVERABLE 3: Show the compiled Bytecode output
    if (DEBUG_MODE) {
        std::cout << "\n[DEBUG] --- Bytecode Output ---\n";
        compiler.disassembleChunk("Chunk");
        std::cout << "\n[VM Output]\n";
    }

    // 4. Virtual Machine Execution (VM)
    VM vm;
    vm.interpret(&chunk);
}

// --- ERROR HANDLING ---
void report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    hadError = true;
}

void error(int line, const std::string& message) {
    report(line, "", message);
}