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
        {"int", Token(TokenType::INTEGER, "int")},
        {"double", Token(TokenType::DOUBLE, "double")}};
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
    void skipWhitespace();
    void skipComment();
    Token nextNumber();
    bool isNextCharID();
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
    AST *assignment_expr();
    AST *declaration();
    AST *type_spec();

    Lexer lexer_;
    Token currToken_;
};

class ASTTraverser {
   public:
    ASTTraverser();
    ASTTraverser(AST *root);

   protected:
    AST *root_;

   private:
    virtual int visit(const AST &node);
    virtual int visitProgram(const Program &node);
    virtual int visitCompound(const Compound &node);
    virtual int visitBinOp(const BinOp &node);
    virtual int visitUnaryOp(const UnaryOp &node);
    virtual int visitAssign(const Assign &node);
    virtual int visitVarDecl(const VarDecl &node);
    virtual int visitType(const Type &node);
    virtual int visitVar(const Var &node);
    virtual int visitNum(const Num &node);
};

class Interpreter : ASTTraverser {
   public:
    Interpreter(AST *root);
    int interpret();
    std::string toString() const;

   private:
    int visit(const AST &node);
    int visitProgram(const Program &node);
    int visitCompound(const Compound &node);
    int visitBinOp(const BinOp &node);
    int visitUnaryOp(const UnaryOp &node);
    int visitAssign(const Assign &node);
    int visitVarDecl(const VarDecl &node);
    int visitType(const Type &node);
    int visitVar(const Var &node);
    int visitNum(const Num &node);

    std::unordered_map<std::string, int> GLOBAL;
};

class Symbol {
   public:
    Symbol(std::string name, std::string type);
    std::string toString();
    std::string name;
    std::string type;
};

class BuiltinTypeSymbol : Symbol {
   public:
    BuiltinTypeSymbol(std::string name);
};

class VarSymbol : Symbol {
   public:
    VarSymbol(std::string name, std::string type);
};

class SymbolTable {
   public:
    SymbolTable();
    std::string toString();
    void define(Symbol &symbol);
    int lookup(std::string name);

   private:
    void initBuiltins();
    std::unordered_map<std::string, int> symbols_;
};

class SymbolTableBuilder : ASTTraverser {
   public:
    SymbolTableBuilder(AST *root);
    std::string toString() const;

   private:
    int visit(const AST &node);
    int visitProgram(const Program &node);
    int visitCompound(const Compound &node);
    int visitBinOp(const BinOp &node);
    int visitUnaryOp(const UnaryOp &node);
    int visitAssign(const Assign &node);
    int visitVarDecl(const VarDecl &node);
    int visitType(const Type &node);
    int visitVar(const Var &node);
    int visitNum(const Num &node);

    SymbolTable symbols_;
};

} // namespace bluefin
