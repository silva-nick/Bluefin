#pragma once

#include <boost/any.hpp>
#include <iostream>
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
    boost::any evaluate(const AST *node);
    bool isEqual(const boost::any &left, const boost::any &right) const;
    void throw_error(Token *token, const std::string &message) const;

    std::unordered_map<std::string, boost::any> global_;
    std::stringstream &buffer_;
};

} // namespace bluefin
