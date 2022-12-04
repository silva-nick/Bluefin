#pragma once

#include "ast.hpp"
#include "token.hpp"

namespace bluefin {

class Lexer {
   public:
    Lexer(std::string expr, std::stringstream &buffer);
    Token *nextToken();
    size_t line;

   private:
    Token *lexNextToken();
    bool hasMoreChars();
    bool tokenHasMoreChars();
    char peek();
    char peekNext();
    bool nextMatches(char c);
    std::string getCurrentTokenString();
    void skipWhitespace();
    Token *handleForwardSlash();
    void skipComment();
    Token *nextNumber();
    bool isNextCharID();
    Token *nextID();
    Token *nextString();
    Token *makeToken(TokenType type);
    Token *makeToken(TokenType, std::string value);

    const std::string expr_;
    size_t tokenStart_;
    size_t tokenLen_;

    std::stringstream &buffer_;
};

// TODO: make this compile time?
const std::unordered_map<std::string, TokenType> RESERVED_KEYWORDS = [] {
    std::unordered_map<std::string, TokenType> map = {
        {"int", TokenType::INTEGER},
        {"double", TokenType::DOUBLE},
        {"string", TokenType::STRING},
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"fun", TokenType::FUN},
        {"for", TokenType::FOR},
        {"if", TokenType::IF},
        {"none", TokenType::NONE},
        {"or", TokenType::OR},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"while", TokenType::WHILE}};
    return map;
}();

class Parser {
   public:
    Parser(Lexer lexer, std::stringstream &buffer);
    AST *parse();

   private:
    void consume(TokenType type);
    AST *program();
    AST *compound_statement();
    std::vector<AST *> statement_list();
    AST *statement();
    AST *additive_expr();
    AST *multiplicative_expr();
    AST *unary_expr();
    AST *primary_expr();
    AST *assignment_expr();
    AST *declaration();
    AST *type_spec();

    void throw_error(const std::string &message);

    Lexer lexer_;
    Token *currToken_;
    std::stringstream &buffer_;
};

} // namespace bluefin
