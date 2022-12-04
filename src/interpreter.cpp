#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

Interpreter::Interpreter(std::stringstream &buffer) : buffer_(buffer) {}

void Interpreter::printVariables() const {
    std::cout << "Global vars: \n";

    for (const auto &x : this->global_) {
        std::cout << x.first + ": " + std::to_string(x.second) << std::endl;
    }
}

void Interpreter::evaluate(const AST *node) const {
    return node->accept((Visitor *)this);
}

void Interpreter::visitProgram(const Program *node) const {
    for (const AST *compound : node->compounds) {
        evaluate(compound);
    }
}

void Interpreter::visitCompound(const Compound *node) const {
    for (AST *statement : node->statements) {
        evaluate(statement);
    }
}

void Interpreter::visitBinOp(const BinOp *node) const {
    switch (node->op->type) {
        case TokenType::PLUS:
            return this->visit(node.left) + this->visit(node.right);
            break;
        case TokenType::MINUS:
            return this->visit(node.left) - this->visit(node.right);
            break;
        case TokenType::MULT:
            return this->visit(node.left) * this->visit(node.right);
            break;
        case TokenType::DIV:
            return this->visit(node.left) / this->visit(node.right);
            break;
        case TokenType::INT_DIV:
            return (double)this->visit(node.left) / this->visit(node.right);
            break;
        case TokenType::REM:
            return this->visit(node.left) % this->visit(node.right);
            break;
        default:
            throw new std::logic_error(
                "Interpreter found invalid binary op" + node.toString());
    }
}

void Interpreter::visitUnaryOp(const UnaryOp *node) {
    printf("%s\n", node.toString().c_str());

    switch (node.token->type) {
        case TokenType::PLUS:
            return +this->visit(node.child);
            break;
        case TokenType::MINUS:
            return -this->visit(node.child);
            break;
        default:
            throw new std::logic_error(
                "Interpreter found invalid unary op" + node.toString());
    }
}

void Interpreter::visitAssign(const Assign *node) {
    printf("%s\n", node.toString().c_str());

    StringToken *token = (StringToken *)node.left.token;
    this->global_.emplace(token->value, this->visit(node.right));
    return 0;
}

void Interpreter::visitVarDecl(const VarDecl *node) {
    printf("%s\n", node.toString().c_str());

    StringToken *token = (StringToken *)node.id.token;
    if (node.expr.token->type == TokenType::END) {
        this->global_.emplace(token->value, -999);
    } else {
        this->global_.emplace(token->value, this->visit(node.expr));
    }

    return 0;
}

void Interpreter::visitType(const Type *node) {
    printf("%s\n", node.toString().c_str());

    return 0;
}

void Interpreter::visitVar(const Var *node) {
    printf("%s\n", node.toString().c_str());

    StringToken *token = (StringToken *)node.token;
    return this->global_.at(token->value); // throws
}

void Interpreter::visitNum(const Num *node) {
    printf("%s\n", node.toString().c_str());

    IntegerToken *token = (IntegerToken *)node.token;
    return token->value;
}

void Interpreter::visitString(const String *node) {
    printf("%s\n", node.toString().c_str());
    return -27;
}

void Interpreter::interpret() {
    printf("\nINTERPRETING...\n");

    this->visit(*this->root_);

    printf("\n%s \n", this->toString().c_str());

    return 0;
}
// end interpreter

} // namespace bluefin
