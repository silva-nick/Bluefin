
#pragma once

#include <functional>
#include <string>
#include <vector>
#include "token.hpp"

namespace bluefin {

enum class ASTType : int {
    BinOp,
    Double,
    Program,
    Var,
    Type,
    Assign,
    String,
    UnaryOp,
    VarDecl,
    NoOp,
    Compound,
    Integer,
};
static constexpr const char *const ASTTypeStrings[12] = {
    "BinOp",
    "Double",
    "Program",
    "Var",
    "Type",
    "Assign",
    "String",
    "UnaryOp",
    "VarDecl",
    "NoOp",
    "Compound",
    "Integer",
};

// Tree node parent class
class AST {
   public:
    AST();
    virtual void accept(Visitor *visitor) const = 0;

   private:
};

// BinOp node
class BinOp : public AST {
   public:
    BinOp(AST *left, Token *op, AST *right);
    AST *left;
    Token *op;
    AST *right;

    void accept(Visitor *visitor) const;

   private:
};

// Double node
class Double : public AST {
   public:
    Double(DoubleToken *value);
    DoubleToken *value;

    void accept(Visitor *visitor) const;

   private:
};

// Program node
class Program : public AST {
   public:
    Program(std::vector<AST *> compounds);
    std::vector<AST *> compounds;

    void accept(Visitor *visitor) const;

   private:
};

// Var node
class Var : public AST {
   public:
    Var(StringToken *varName);
    StringToken *varName;

    void accept(Visitor *visitor) const;

   private:
};

// Type node
class Type : public AST {
   public:
    Type(StringToken *typeName);
    StringToken *typeName;

    void accept(Visitor *visitor) const;

   private:
};

// Assign node
class Assign : public AST {
   public:
    Assign(AST *left, Token *op, AST *right);
    AST *left;
    Token *op;
    AST *right;

    void accept(Visitor *visitor) const;

   private:
};

// String node
class String : public AST {
   public:
    String(StringToken *value);
    StringToken *value;

    void accept(Visitor *visitor) const;

   private:
};

// UnaryOp node
class UnaryOp : public AST {
   public:
    UnaryOp(AST *node, Token *op);
    AST *node;
    Token *op;

    void accept(Visitor *visitor) const;

   private:
};

// VarDecl node
class VarDecl : public AST {
   public:
    VarDecl(AST *typeNode, AST *id, AST *expr);
    AST *typeNode;
    AST *id;
    AST *expr;

    void accept(Visitor *visitor) const;

   private:
};

// NoOp node
class NoOp : public AST {
   public:
    NoOp();

    void accept(Visitor *visitor) const;

   private:
};

// Compound node
class Compound : public AST {
   public:
    Compound(std::vector<AST *> statements);
    std::vector<AST *> statements;

    void accept(Visitor *visitor) const;

   private:
};

// Integer node
class Integer : public AST {
   public:
    Integer(IntegerToken *value);
    IntegerToken *value;

    void accept(Visitor *visitor) const;

   private:
};

// Abstract visitor class
class Visitor {
   public:
    virtual void visitBinOp(const BinOp *node) const = 0;
    virtual void visitDouble(const Double *node) const = 0;
    virtual void visitProgram(const Program *node) const = 0;
    virtual void visitVar(const Var *node) const = 0;
    virtual void visitType(const Type *node) const = 0;
    virtual void visitAssign(const Assign *node) const = 0;
    virtual void visitString(const String *node) const = 0;
    virtual void visitUnaryOp(const UnaryOp *node) const = 0;
    virtual void visitVarDecl(const VarDecl *node) const = 0;
    virtual void visitNoOp(const NoOp *node) const = 0;
    virtual void visitCompound(const Compound *node) const = 0;
    virtual void visitInteger(const Integer *node) const = 0;
};

} // namespace bluefin