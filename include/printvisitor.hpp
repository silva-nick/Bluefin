#pragma once

#include <string.h>
#include <iostream>
#include "ast.hpp"

namespace bluefin {

class PrintVisitor : public Visitor {
   public:
    PrintVisitor() = default;
    ~PrintVisitor() = default;

    void print(const AST *root);

    boost::any visitBinOp(const BinOp *node) override;
    boost::any visitProgram(const Program *node) override;
    boost::any visitVar(const Var *node) override;
    boost::any visitType(const Type *node) override;
    boost::any visitAssign(const Assign *node) override;
    boost::any visitString(const String *node) override;
    boost::any visitUnaryOp(const UnaryOp *node) override;
    boost::any visitVarDecl(const VarDecl *node) override;
    boost::any visitDouble(const Double *node) override;
    boost::any visitInteger(const Integer *node) override;
    boost::any visitNoOp(const NoOp *node) override;
    boost::any visitCompound(const Compound *node) override;

   private:
};

} // namespace bluefin
