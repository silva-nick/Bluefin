#pragma once

#include "ast.hpp"

namespace bluefin {

class ASTTraverser {
   public:
    ASTTraverser();
    ASTTraverser(AST *root);

   protected:
    AST *root_;
    int visit(const AST &node);

   private:
    virtual int visitProgram(const Program &node) = 0;
    virtual int visitCompound(const Compound &node) = 0;
    virtual int visitBinOp(const BinOp &node) = 0;
    virtual int visitUnaryOp(const UnaryOp &node) = 0;
    virtual int visitAssign(const Assign &node) = 0;
    virtual int visitVarDecl(const VarDecl &node) = 0;
    virtual int visitType(const Type &node) = 0;
    virtual int visitVar(const Var &node) = 0;
    virtual int visitNum(const Num &node) = 0;
};

} // namespace bluefin