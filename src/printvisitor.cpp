#include "printvisitor.hpp"

namespace bluefin {

void PrintVisitor::print(const AST *root) const {
    root->accept((Visitor *)this);
}

void PrintVisitor::visitBinOp(const BinOp *node) const {
    std::cout << "(" + ((StringToken *)node->op)->value + " ";
    node->left->accept((Visitor *)this);
    node->right->accept((Visitor *)this);
    std::cout << ")";
}

void PrintVisitor::visitProgram(const Program *node) const {
    std::cout << "(";
    for (const AST *compound : node->compounds) {
        compound->accept((Visitor *)this);
    }
    std::cout << ")";
}

void PrintVisitor::visitVar(const Var *node) const {
    std::cout << node->varName->value;
}

void PrintVisitor::visitType(const Type *node) const {
    std::cout << node->typeName->value;
}

void PrintVisitor::visitAssign(const Assign *node) const {
    std::cout << "(" + ((StringToken *)node->op)->value + " ";
    node->left->accept((Visitor *)this);
    node->right->accept((Visitor *)this);
    std::cout << ")";
}

void PrintVisitor::visitString(const String *node) const {
    std::cout << node->value->value;
}

void PrintVisitor::visitUnaryOp(const UnaryOp *node) const {
    std::cout << "(" + ((StringToken *)node->op)->value + " ";
    node->node->accept((Visitor *)this);
    std::cout << ")";
}

void PrintVisitor::visitVarDecl(const VarDecl *node) const {
    std::cout << "(";
    node->typeNode->accept((Visitor *)this);
    node->id->accept((Visitor *)this);
    node->expr->accept((Visitor *)this);
    std::cout << ")";
}

void PrintVisitor::visitDouble(const Double *node) const {
    std::cout << node->value->value;
}

void PrintVisitor::visitInteger(const Integer *node) const {
    std::cout << node->value->value;
}

void PrintVisitor::visitNoOp(const NoOp *node) const {
    std::cout << " ";
}

void PrintVisitor::visitCompound(const Compound *node) const {
    std::cout << "(";
    for (const AST *statement : node->statements) {
        statement->accept((Visitor *)this);
    }
    std::cout << ")";
}

} // namespace bluefin
