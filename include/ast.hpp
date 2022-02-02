#pragma once

#include <functional>
#include <string>
#include <vector>

namespace bluefin {

enum class TokenType : int {
    END = 0,
    BSTR,
    BEND,
    PSTR,
    PEND,
    SEMI,
    INTEGER,
    ID,
    ASSIGN,
    PLUS,
    MINUS,
    MULT,
    DIV,
    REM
};
static constexpr const char *const TokenTypeStrings[14] = {
    "END",
    "BSTR",
    "BEND",
    "PSTR",
    "PEND",
    "SEMI",
    "INTEGER",
    "ID",
    "ASSIGN",
    "PLUS",
    "MINUS",
    "MULT",
    "DIV",
    "REM"};

// Token definition
class Token {
   public:
    Token(TokenType type, std::string value);
    Token();
    TokenType type;
    std::string value;
    std::string toString() const;

   private:
    std::string getTokenTypeString() const;
};

enum class ASTType : int {
    Program,
    Compound,
    Assign,
    BinOp,
    UnaryOp,
    NoOp,
    Var,
    Num
};
static constexpr const char *const ASTTypeStrings[8] =
    {"Program", "Compound", "Assign", "BinOp", "UnaryOp", "NoOp", "Var", "Num"};

// Tree node parent class
class AST {
   public:
    AST();
    AST(Token value, ASTType nodeType);

    Token token;
    ASTType type;

    virtual std::string toString() const;

   private:
};

// Program root node
class Program : public AST {
   public:
    Program();

    // Compound
    std::vector<std::reference_wrapper<AST>> blocks;

    std::string toString() const;

   private:
};

// Compound expression node
class Compound : public AST {
   public:
    Compound();

    // assign or no-op
    std::vector<std::reference_wrapper<AST>> children;

    std::string toString() const;

   private:
};

// Assignment
class Assign : public AST {
   public:
    Assign(AST &left, Token op, AST &right);

    AST &left; // Variable
    AST &right; // Expression

    std::string toString() const;
};

// Binary operators
class BinOp : public AST {
   public:
    BinOp(AST &left, Token op, AST &right);

    AST &left;
    AST &right;

    std::string toString() const;

   private:
};

// Unary operators
class UnaryOp : public AST {
   public:
    UnaryOp(AST &node, Token op);

    AST &child;

    std::string toString() const;

   private:
};

// Empty space / lines
class NoOp : public AST {
   public:
    NoOp();

   private:
};

// Variables
class Var : public AST {
   public:
    Var(Token token);

    std::string toString() const;

   private:
};

// Numbers / terminals
class Num : public AST {
   public:
    Num(Token token);

    std::string toString() const;

   private:
};

} // namespace bluefin
