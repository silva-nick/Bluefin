#pragma once

#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "ast.hpp"
#include "asttraverser.hpp"
#include "symboltable.hpp"
#include "token.hpp"

namespace bluefin {

class Interpreter : public ASTTraverser {
   public:
    Interpreter(AST *root, std::stringstream &buffer);
    int interpret();
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

    std::unordered_map<std::string, int> global_;
    std::stringstream &buffer_;
};

} // namespace bluefin
