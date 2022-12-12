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
};

} // namespace bluefin
