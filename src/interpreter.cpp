#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

Interpreter::Interpreter(Parser parser) : parser_(std::move(parser)) {}

std::string Interpreter::toString() const {
    std::string out;

    for (const auto &x : this->GLOBAL) {
        out += x.first + ": " + std::to_string(x.second) + "\n";
    }

    return out;
}

int Interpreter::visit(const AST &node) {
    printf("visiting %s \n", ASTTypeStrings[static_cast<int>(node.type)]);
    switch (node.type) {
        case ASTType::Program:
            return visitProgram(static_cast<const Program &>(node));
        case ASTType::Compound:
            return visitCompound(static_cast<const Compound &>(node));
        case ASTType::BinOp:
            return visitBinOp(static_cast<const BinOp &>(node));
        case ASTType::UnaryOp:
            return visitUnaryOp(static_cast<const UnaryOp &>(node));
        case ASTType::NoOp:
            return 0;
        case ASTType::Assign:
            return visitAssign(static_cast<const Assign &>(node));
        case ASTType::VarDecl:
            return visitVarDecl(static_cast<const VarDecl &>(node));
        case ASTType::Type:
            return visitType(static_cast<const Type &>(node));
        case ASTType::Var:
            return visitVar(static_cast<const Var &>(node));
        case ASTType::Num:
            return visitNum(static_cast<const Num &>(node));
        default:
            assert(0);
    }
}

int Interpreter::visitProgram(const Program &node) {
    printf("%s\n", node.toString().c_str());

    for (AST &block : node.blocks) {
        this->visit(block);
    }
    return 0;
}

int Interpreter::visitCompound(const Compound &node) {
    printf("%s\n", node.toString().c_str());

    for (AST &child : node.children) {
        this->visit(child);
    }
    return 0;
}

int Interpreter::visitBinOp(const BinOp &node) {
    printf("%s\n", node.toString().c_str());

    switch (node.token.type) {
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
            assert(0);
    }
}

int Interpreter::visitUnaryOp(const UnaryOp &node) {
    printf("%s\n", node.toString().c_str());

    switch (node.token.type) {
        case TokenType::PLUS:
            return +this->visit(node.child);
            break;
        case TokenType::MINUS:
            return -this->visit(node.child);
            break;
        default:
            assert(0);
    }
}

int Interpreter::visitAssign(const Assign &node) {
    printf("%s\n", node.toString().c_str());

    this->GLOBAL.emplace(node.left.token.value, this->visit(node.right));
    return 0;
}

int Interpreter::visitVarDecl(const VarDecl &node) {
    printf("%s\n", node.toString().c_str());

    if (node.expr.token.type == TokenType::END) {
        this->GLOBAL.emplace(node.id.token.value, -999);
    } else {
        this->GLOBAL.emplace(node.id.token.value, this->visit(node.expr));
    }

    return 0;
}

int Interpreter::visitType(const Type &node) {
    printf("%s\n", node.toString().c_str());

    return 0;
}

int Interpreter::visitVar(const Var &node) {
    printf("%s\n", node.toString().c_str());

    return this->GLOBAL.at(node.token.value); // throws
}

int Interpreter::visitNum(const Num &node) {
    printf("%s\n", node.toString().c_str());

    return std::stoi(node.token.value);
}

int Interpreter::interpret() {
    // search for new keyword
    AST *root = this->parser_.parse();

    printf("\nINTERPRETING...\n");
    printf("root node %s \n", root->token.toString().c_str());

    this->visit(*root);

    printf("symbol table \n%s \n", this->toString().c_str());

    return 0;
}
// end interpreter

} // namespace bluefin
