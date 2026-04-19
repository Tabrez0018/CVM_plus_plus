#include "Scanner.hpp"
// Tell the compiler that 'error' is defined in another file (main.cpp)
extern void error(int line, const std::string &message);

// Constructor implementation
Scanner::Scanner(const std::string &source) : source(source)
{
    // In C++, std::vector is automatically initialized as empty.
    // We do not need the equivalent of `= new ArrayList<>()`.
}
bool Scanner::isAtEnd() const
{
    // std::string::length() returns the number of characters in the string.
    return current >= source.length();
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        // We are at the beginning of the next lexeme.
        start = current;
        scanToken();
    }

    // tokens.push_back is the C++ equivalent of Java's tokens.add()
    // Notice we use TokenType::EOF_TOKEN, matching the enum class in your token.hpp
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", line));

    return tokens;
}

char Scanner::advance()
{
    // Returns the current character, THEN increments the 'current' integer.
    // source[index] is the fast C++ way to get a char at a specific index.
    return source[current++];
}

bool Scanner::match(char expected)
{
    // 1. If we are out of code, it definitely doesn't match.
    if (isAtEnd())
        return false;

    // 2. If the next character isn't what we want, don't consume it!
    if (source[current] != expected)
        return false;

    // 3. It's a match! Consume the character and return true.
    current++;
    return true;
}
char Scanner::peek() const
{
    // If we are out of code, return the null terminator character.
    // This is the standard C/C++ way to represent the end of a string safely.
    if (isAtEnd())
        return '\0';

    // Otherwise, return the character we are currently pointing at,
    // but DO NOT increment 'current'!
    return source[current];
}
void Scanner::string() {
    // Keep consuming characters until we hit the closing quote or EOF
    while (peek() != '"' && !isAtEnd()) {
        // CVM++ supports multi-line strings!
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        error(line, "Unterminated string.");
        return;
    }

    // Consume the closing ".
    advance();

    // Trim the surrounding quotes to get the actual value
    // In C++, substr takes (start_index, length)
    int valueLength = (current - 1) - (start + 1);
    std::string value = source.substr(start + 1, valueLength);
    
    // Because your current C++ Token class doesn't have a separate "literal" 
    // object like Java does, we will just store the trimmed string directly 
    // into the 'lexeme' field for now. 
    tokens.push_back(Token(TokenType::STRING, value, line));
}
bool Scanner::isDigit(char c) const {
    // Like Bob Nystrom, we avoid C++'s built-in std::isdigit() from <cctype> 
    // because it can behave weirdly depending on system locales. 
    // This strictly limits our numbers to standard ASCII 0-9.
    return c >= '0' && c <= '9';
}

char Scanner::peekNext() const {
    // Look TWO characters ahead. 
    // If that pushes us past the end of the file, return the null terminator.
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Scanner::number() {
    // Consume digits as long as we keep seeing them
    while (isDigit(peek())) {
        advance();
    }

    // Look for a fractional part (a decimal point followed by another digit)
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();

        // Consume the fractional digits
        while (isDigit(peek())) {
            advance();
        }
    }

    // Extract the full number string (e.g., "123" or "12.34")
    std::string value = source.substr(start, current - start);
    
    // Push the NUMBER token. (We will convert this string to an actual integer 
    // or double in the compiler phase!)
    tokens.push_back(Token(TokenType::NUMBER, value, line));
}
void Scanner::addToken(TokenType type)
{
    // 1. Extract the lexeme string from the original source code
    // C++ substr takes (start_index, length_of_substring)
    std::string text = source.substr(start, current - start);

    // 2. Add the new token to our vector
    tokens.push_back(Token(type, text, line));
}

bool Scanner::isAlpha(char c) const {
    // Just like isDigit, we write our own to strictly enforce standard ASCII 
    // and allow underscores (_) so users can name variables like 'my_var'
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

void Scanner::identifier() {
    // Keep consuming characters as long as they are letters, numbers, or underscores
    while (isAlphaNumeric(peek())) {
        advance();
    }

    // Extract the word we just scanned
    std::string text = source.substr(start, current - start);

    // Check if the word is a reserved keyword in our map
    TokenType type = TokenType::IDENTIFIER; // Assume it's a variable name first
    
    auto match = keywords.find(text);
    if (match != keywords.end()) {
        // If it is in the map, use the specific keyword type instead!
        type = match->second;
    }

    // Add the token
    tokens.push_back(Token(type, text, line));
}

void Scanner::scanToken()
{
    char c = advance();

    switch (c)
    {
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
    case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case '*':
        addToken(TokenType::STAR);
        break;

    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/':
        if (match('/'))
        {
            // A comment goes until the end of the line.
            // We use peek() to check for the newline without consuming it.
            while (peek() != '\n' && !isAtEnd())
            {
                advance();
            }
        }
        else
        {
            addToken(TokenType::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;

    case '\n':
        line++;
        break;
    case '"': string(); break;
    default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                error(line, "Unexpected character.");
            }
            break;
        // We will add the default case (for errors and whitespace) later!
    }
}