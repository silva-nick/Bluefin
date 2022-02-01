#pragma once

#include <assert.h>
#include <string>
#include <unordered_map>
#include "ast.hpp"

namespace bluefin {

int run(const std::string &expr);

// TODO: make this compile time?
const std::unordered_map<std::string, Token> RESERVED_KEYWORDS = [] {
    std::unordered_map<std::string, Token> map = {
        {"TEST", Token(TokenType::ID, "TEST")}};
    return map;
}();

class Lexer {
   public:
    Lexer(std::string expr);
    Token nextToken();
    const std::string expr_;

   private:
    bool hasMoreChars();
    bool tokenHasMoreChars();
    char peek();
    std::string getCurrentTokenString();
    Token nextInteger();
    Token nextID();
    size_t tokenStart_;
    size_t tokenLen_;
};

class Parser {
   public:
    Parser(Lexer lexer);
    AST *parse();

   private:
    void consume(TokenType type);
    AST *additive();
    AST *multiplicative();
    AST *primary();
    Lexer lexer_;
    Token currToken_;
};

class Interpreter {
   public:
    Interpreter(Parser parser);
    int interpret();

   private:
    Parser parser_;
    int visit(const AST &node) const;
    int visitBinOp(const BinOp &node) const;
    int visitUnaryOp(const UnaryOp &node) const;
    int visitNum(const Num &node) const;
};

} // namespace bluefin
