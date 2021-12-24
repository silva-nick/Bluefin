#pragma once

#include <assert.h>
#include <string>

namespace bluefin {

enum class TokenType : int { END=0, INTEGER, PLUS, MINUS };
static constexpr const char* const TokenTypeStrings[3] = {"END", "INTEGER", "PLUS"};

class Token {
public:
    Token(TokenType type, std::string value);
    Token();
    TokenType type;
    std::string value;
    std::string toString();
private:
    std::string getTokenTypeString();
};

class Interpreter {
    public:
        Interpreter(std::string expr);
        int parse();
    private:
        bool hasMoreTokens();
        Token nextToken();
        void consume(TokenType type);
        std::string expr;
        Token currToken_;
        size_t tokenStart_;
        size_t tokenEnd_;
};

} // namespace bluefin
