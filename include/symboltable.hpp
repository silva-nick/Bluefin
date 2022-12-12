#pragma once

#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "ast.hpp"

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
    Symbol lookup(const StringToken *name, std::stringstream &buffer) const;

   private:
    void initBuiltins();
    std::unordered_map<std::string, Symbol> symbols_;
};

class SymbolTableBuilder : Visitor {
   public:
    SymbolTableBuilder(std::stringstream &buffer);
    void build(const AST *root);
    std::string toString() const;

    boost::any visitBinOp(const BinOp *node);
    boost::any visitProgram(const Program *node);
    boost::any visitVar(const Var *node);
    boost::any visitType(const Type *node);
    boost::any visitAssign(const Assign *node);
    boost::any visitString(const String *node);
    boost::any visitUnaryOp(const UnaryOp *node);
    boost::any visitVarDecl(const VarDecl *node);
    boost::any visitDouble(const Double *node);
    boost::any visitInteger(const Integer *node);
    boost::any visitNoOp(const NoOp *node);
    boost::any visitCompound(const Compound *node);

   private:
    SymbolTable symbols_;
    std::stringstream &buffer_;
};

} // namespace bluefin
