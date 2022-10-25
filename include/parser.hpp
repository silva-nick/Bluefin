#pragma once

#include "ast.hpp"
#include "token.hpp"

namespace bluefin {

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
    void skipWhitespace();
    Token handleForwardSlash();
    void skipComment();
    Token nextNumber();
    bool isNextCharID();
    Token nextID();
    Token makeToken(TokenType type);
    Token makeToken(TokenType, std::string value);
    size_t tokenStart_;
    size_t tokenLen_;
    size_t line_;
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
    AST *assignment_expr();
    AST *declaration();
    AST *type_spec();

    Lexer lexer_;
    Token currToken_;
};

} // namespace bluefin