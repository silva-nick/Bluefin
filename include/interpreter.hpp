#pragma once

#include <assert.h>
#include <boost/any.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "ast.hpp"
#include "symboltable.hpp"
#include "token.hpp"

namespace bluefin {

class Interpreter : public Visitor {
   public:
    Interpreter(std::stringstream &buffer);
    void interpret(const AST *root);
    void printVariables() const;

    boost::any visitBinOp(const BinOp *node) const;
    boost::any visitProgram(const Program *node) const;
    boost::any visitVar(const Var *node) const;
    boost::any visitType(const Type *node) const;
    boost::any visitAssign(const Assign *node) const;
    boost::any visitString(const String *node) const;
    boost::any visitUnaryOp(const UnaryOp *node) const;
    boost::any visitVarDecl(const VarDecl *node) const;
    boost::any visitDouble(const Double *node) const;
    boost::any visitInteger(const Integer *node) const;
    boost::any visitNoOp(const NoOp *node) const;
    boost::any visitCompound(const Compound *node) const;

   private:
    boost::any evaluate(const AST *node) const;
    void throw_error(Token *token, const std::string &message) const;

    std::unordered_map<std::string, int> global_;
    std::stringstream &buffer_;
};

} // namespace bluefin
