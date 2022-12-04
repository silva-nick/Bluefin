#pragma once

#include <string.h>
#include <iostream>
#include "ast.hpp"

namespace bluefin {
class PrintVisitor : Visitor {
   public:
    PrintVisitor();
    ~PrintVisitor();

    void print(const AST *root) const;
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
};

} // namespace bluefin
