#pragma once

#include <boost/any.hpp>
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
class Visitor;

// Tree node parent class
class AST {
   public:
    AST();
    virtual boost::any accept(Visitor *visitor) const = 0;

   private:
};

// BinOp node
class BinOp : public AST {
   public:
    BinOp(AST *left, Token *op, AST *right);
    AST *left;
    Token *op;
    AST *right;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Double node
class Double : public AST {
   public:
    Double(DoubleToken *value);
    DoubleToken *value;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Program node
class Program : public AST {
   public:
    Program(std::vector<AST *> compounds);
    std::vector<AST *> compounds;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Var node
class Var : public AST {
   public:
    Var(StringToken *varName);
    StringToken *varName;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Type node
class Type : public AST {
   public:
    Type(StringToken *typeName);
    StringToken *typeName;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Assign node
class Assign : public AST {
   public:
    Assign(AST *left, Token *op, AST *right);
    AST *left;
    Token *op;
    AST *right;

    boost::any accept(Visitor *visitor) const;

   private:
};

// String node
class String : public AST {
   public:
    String(StringToken *value);
    StringToken *value;

    boost::any accept(Visitor *visitor) const;

   private:
};

// UnaryOp node
class UnaryOp : public AST {
   public:
    UnaryOp(AST *node, Token *op);
    AST *node;
    Token *op;

    boost::any accept(Visitor *visitor) const;

   private:
};

// VarDecl node
class VarDecl : public AST {
   public:
    VarDecl(AST *typeNode, AST *id, AST *expr);
    AST *typeNode;
    AST *id;
    AST *expr;

    boost::any accept(Visitor *visitor) const;

   private:
};

// NoOp node
class NoOp : public AST {
   public:
    NoOp();

    boost::any accept(Visitor *visitor) const;

   private:
};

// Compound node
class Compound : public AST {
   public:
    Compound(std::vector<AST *> statements);
    std::vector<AST *> statements;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Integer node
class Integer : public AST {
   public:
    Integer(IntegerToken *value);
    IntegerToken *value;

    boost::any accept(Visitor *visitor) const;

   private:
};

// Abstract visitor class
class Visitor {
   public:
    virtual boost::any visitBinOp(const BinOp *node) const = 0;
    virtual boost::any visitDouble(const Double *node) const = 0;
    virtual boost::any visitProgram(const Program *node) const = 0;
    virtual boost::any visitVar(const Var *node) const = 0;
    virtual boost::any visitType(const Type *node) const = 0;
    virtual boost::any visitAssign(const Assign *node) const = 0;
    virtual boost::any visitString(const String *node) const = 0;
    virtual boost::any visitUnaryOp(const UnaryOp *node) const = 0;
    virtual boost::any visitVarDecl(const VarDecl *node) const = 0;
    virtual boost::any visitNoOp(const NoOp *node) const = 0;
    virtual boost::any visitCompound(const Compound *node) const = 0;
    virtual boost::any visitInteger(const Integer *node) const = 0;
};

} // namespace bluefin