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
    AST *program();
    AST *compound_statement();
    std::vector<std::reference_wrapper<AST>> statement_list();
    AST *statement();
    AST *additive_expr();
    AST *multiplicative_expr();
    AST *unary_expr();
    AST *primary_expr();
    // AST *expr();
    AST *assignment_expr();

    Lexer lexer_;
    Token currToken_;
};

class Interpreter {
   public:
    Interpreter(Parser parser);
    int interpret();
    std::string toString() const;

   private:
    int visit(const AST &node);
    int visitCompound(const Compound &node);
    int visitBinOp(const BinOp &node);
    int visitUnaryOp(const UnaryOp &node);
    int visitAssign(const Assign &node);
    int visitVar(const Var &node);
    int visitNum(const Num &node);

    Parser parser_;
    std::unordered_map<std::string, int> GLOBAL;
};

} // namespace bluefin
