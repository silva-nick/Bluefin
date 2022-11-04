#pragma once

#include <functional>
#include <string>
#include <vector>
#include "token.hpp"

namespace bluefin {

enum class ASTType : int {
    Program,
    Compound,
    Assign,
    BinOp,
    UnaryOp,
    NoOp,
    VarDecl,
    Type,
    Var,
    Num,
    String
};
static constexpr const char *const ASTTypeStrings[11] = {
    "Program",
    "Compound",
    "Assign",
    "BinOp",
    "UnaryOp",
    "NoOp",
    "VarDecl",
    "Type",
    "Var",
    "Num",
    "String"
};

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

// Variable declaration node
class VarDecl : public AST {
   public:
    VarDecl(AST &typeNode, AST &varNode, AST &rhsNode);

    std::string toString() const;

    AST &typeNode;
    AST &id;
    AST &expr;

   private:
};

// Variable types
class Type : public AST {
   public:
    Type(Token token);

    std::string toString() const;

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

// strings
class String : public AST {
   public:
    String(Token token);

    std::string toString() const;

   private:
};

} // namespace bluefin
