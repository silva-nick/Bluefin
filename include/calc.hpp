#pragma once

#include <assert.h>
#include <string>

namespace bluefin {

enum class TokenType : int { END=0, INTEGER, PLUS, MINUS, MULT, DIV };
static constexpr const char* const TokenTypeStrings[6] = {"END", "INTEGER", "PLUS", "MINUS", "MULT", "DIV"};

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
        bool hasMoreChars();
        bool tokenHasMoreChars();
        std::string getCurrentTokenString();
        Token findToken();
        Token nextToken();
        Token nextInteger();
        void consume(TokenType type);
        std::string expr;
        Token currToken_;
        size_t tokenStart_;
        size_t tokenLen_;
};

} // namespace bluefin
