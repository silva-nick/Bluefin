#include "printvisitor.hpp"

namespace bluefin {

void PrintVisitor::print(const AST *root) {
    printf("Printing AST:\n");
    root->accept((Visitor *)this);
}

boost::any PrintVisitor::visitBinOp(const BinOp *node) {
    std::cout << "(" + ((StringToken *)node->op)->value + " ";
    node->left->accept((Visitor *)this);
    node->right->accept((Visitor *)this);
    std::cout << ")";
    return boost::any();
}

boost::any PrintVisitor::visitProgram(const Program *node) {
    std::cout << std::endl << "(";
    for (const AST *compound : node->compounds) {
        compound->accept((Visitor *)this);
    }
    std::cout << ")" << std::endl;
    return boost::any();
}

boost::any PrintVisitor::visitVar(const Var *node) {
    std::cout << node->varName->value;
    return boost::any();
}

boost::any PrintVisitor::visitType(const Type *node) {
    std::cout << node->typeName->value;
    return boost::any();
}

boost::any PrintVisitor::visitAssign(const Assign *node) {
    std::cout << "(" + ((StringToken *)node->op)->value + " ";
    node->left->accept((Visitor *)this);
    node->right->accept((Visitor *)this);
    std::cout << ")";
    return boost::any();
}

boost::any PrintVisitor::visitString(const String *node) {
    std::cout << node->value->value;
    return boost::any();
}

boost::any PrintVisitor::visitUnaryOp(const UnaryOp *node) {
    std::cout << "(" + ((StringToken *)node->op)->value + " ";
    node->node->accept((Visitor *)this);
    std::cout << ")";
    return boost::any();
}

boost::any PrintVisitor::visitVarDecl(const VarDecl *node) {
    std::cout << "(";
    node->typeNode->accept((Visitor *)this);
    node->id->accept((Visitor *)this);
    node->expr->accept((Visitor *)this);
    std::cout << ")";
    return boost::any();
}

boost::any PrintVisitor::visitDouble(const Double *node) {
    std::cout << node->value->value;
    return boost::any();
}

boost::any PrintVisitor::visitInteger(const Integer *node) {
    std::cout << node->value->value;
    return boost::any();
}

boost::any PrintVisitor::visitNoOp(const NoOp *node) {
    std::cout << " ";
    return boost::any();
}

boost::any PrintVisitor::visitCompound(const Compound *node) {
    std::cout << "(";
    for (const AST *statement : node->statements) {
        statement->accept((Visitor *)this);
    }
    std::cout << ")";
    return boost::any();
}

} // namespace bluefin
