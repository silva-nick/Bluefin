#pragma once

#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "ast.hpp"
#include "asttraverser.hpp"

namespace bluefin {

class Symbol {
   public:
    Symbol(){};
    Symbol(std::string name);
    std::string toString() const;
    std::string name;
};

class BuiltinTypeSymbol : public Symbol {
   public:
    BuiltinTypeSymbol(std::string name);
};

class VarSymbol : public Symbol {
   public:
    VarSymbol(std::string name, Symbol type);
    std::string toString() const;
    const Symbol &type;
};

class SymbolTable {
   public:
    SymbolTable();
    std::string toString() const;
    void define(const Symbol &symbol);
    Symbol lookup(const std::string &name);

   private:
    void initBuiltins();
    std::unordered_map<std::string, Symbol> symbols_;
};

class SymbolTableBuilder : public ASTTraverser {
   public:
    SymbolTableBuilder(AST *root, std::stringstream &buffer);
    void build();
    std::string toString() const;

   private:
    int visitProgram(const Program &node);
    int visitCompound(const Compound &node);
    int visitBinOp(const BinOp &node);
    int visitUnaryOp(const UnaryOp &node);
    int visitAssign(const Assign &node);
    int visitVarDecl(const VarDecl &node);
    int visitType(const Type &node);
    int visitVar(const Var &node);
    int visitNum(const Num &node);
    int visitString(const String &node);

    SymbolTable symbols_;
    std::stringstream &buffer_;
};

} // namespace bluefin
