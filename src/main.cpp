#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// These headers don't exist yet, but we need them for the compiler to know 
// what Token and Scanner are. We will build them next!
#include "Token.h"
#include "Scanner.h"

// 1. Global State
inline bool hadError = false;

// 2. Forward Declarations (Tell the compiler these exist further down)
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
        // argv[1] is a char array, but C++ automatically converts it 
        // to a std::string to match our runFile signature.
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

    if (hadError) std::exit(65);
}

void runPrompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }
        run(line);
        hadError = false;
    }
}

void run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // For now, just print the tokens.
    for (const Token& token : tokens) {
        std::cout << token.toString() << "\n";
    }
}

void report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    hadError = true;
}

void error(int line, const std::string& message) {
    report(line, "", message);
}