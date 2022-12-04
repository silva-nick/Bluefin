#pragma once

#include <assert.h>
#include <boost/any.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "ast.hpp"
#include "asttraverser.hpp"
#include "symboltable.hpp"
#include "token.hpp"

namespace bluefin {

class Interpreter : public Visitor {
   public:
    Interpreter(std::stringstream &buffer);
    void interpret(const AST *root);
    void printVariables() const;

    void visitBinOp(const BinOp *node) const;
    void visitProgram(const Program *node) const;
    void visitVar(const Var *node) const;
    void visitType(const Type *node) const;
    void visitAssign(const Assign *node) const;
    void visitString(const String *node) const;
    void visitUnaryOp(const UnaryOp *node) const;
    void visitVarDecl(const VarDecl *node) const;
    void visitDouble(const Double *node) const;
    void visitInteger(const Integer *node) const;
    void visitNoOp(const NoOp *node) const;
    void visitCompound(const Compound *node) const;

   private:
    void evaluate(const AST *node) const;

    std::unordered_map<std::string, int> global_;
    std::stringstream &buffer_;
};

} // namespace bluefin
