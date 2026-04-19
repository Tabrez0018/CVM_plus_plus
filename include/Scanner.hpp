#pragma once
#include <string>
#include <vector>
#include <unordered_map> // To replace Java's HashMap for keywords later
#include "Token.hpp"

class Scanner {
private:
    std::string source;
    std::vector<Token> tokens;

public:
    // Constructor
    Scanner(const std::string& source);

    // We know from main.cpp that we will need this function!
    std::vector<Token> scanTokens();
};