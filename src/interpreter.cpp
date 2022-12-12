#include "interpreter.hpp"
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

boost::any Interpreter::evaluate(const AST *node) const {
    return node->accept((Visitor *)this);
}

void Interpreter::throw_error(Token *token, const std::string &message) const {
    error(token->line, message, buffer_);
}

boost::any Interpreter::visitProgram(const Program *node) const {
    for (const AST *compound : node->compounds) {
        evaluate(compound);
    }
}

boost::any Interpreter::visitCompound(const Compound *node) const {
    for (AST *statement : node->statements) {
        evaluate(statement);
    }
}

boost::any Interpreter::visitBinOp(const BinOp *node) const {
    boost::any left = evaluate(node->left);
    boost::any right = evaluate(node->right);

    bool isInt = node->left->type == ASTType::Integer &&
        node->right->type == ASTType::Integer;

    double rawRes;

    switch (node->op->type) {
        case TokenType::PLUS:
            if (node->left->type == ASTType::Double &&
                node->right->type == ASTType::Double) {
                rawRes = boost::any_cast<double>(left) +
                    boost::any_cast<double>(right);
            }
            
            if (node->left->type == ASTType::String &&
                node->right->type == ASTType::String) {
                return boost::any_cast<std::string>(left) +
                    boost::any_cast<std::string>(right);
            }
            break;
        case TokenType::MINUS:
            rawRes = boost::any_cast<double>(left) -
                boost::any_cast<double>(right);
            break;
        case TokenType::MULT:
            rawRes = boost::any_cast<double>(left) *
                boost::any_cast<double>(right);
            break;
        case TokenType::DIV:
            rawRes = boost::any_cast<double>(left) /
                boost::any_cast<double>(right);
            break;
        case TokenType::INT_DIV:
            return boost::any_cast<double>(left) /
                boost::any_cast<double>(right);
            break;
        case TokenType::REM:
            return boost::any_cast<int>(left) %
                boost::any_cast<int>(right);
            break;
        case TokenType::GREATER:
            return boost::any_cast<double>(left) >
                boost::any_cast<double>(right);
        case TokenType::GREATER_EQ:
            return boost::any_cast<double>(left) >=
                boost::any_cast<double>(right);
        case TokenType::LESS:
            return boost::any_cast<double>(left) <
                boost::any_cast<double>(right);
        case TokenType::LESS_EQ:
            return boost::any_cast<double>(left) <=
                boost::any_cast<double>(right);
        case TokenType::EQ:
            return right.type();
        default:
            throw new std::logic_error(
                "Interpreter found invalid binary op" + node->toString());
    }

    return isInt ? (int)rawRes : rawRes;
}

boost::any Interpreter::visitUnaryOp(const UnaryOp *node) {
    printf("%s\n", node.toString().c_str());

    switch (node.token->type) {
        case TokenType::PLUS:
            return +evaluate(node.child);
            break;
        case TokenType::MINUS:
            return -evaluate(node.child);
            break;
        default:
            throw new std::logic_error(
                "Interpreter found invalid unary op" + node.toString());
    }
}

boost::any Interpreter::visitAssign(const Assign *node) {
    printf("%s\n", node.toString().c_str());

    StringToken *token = (StringToken *)node.left.token;
    this->global_.emplace(token->value, evaluate(node.right));
    return 0;
}

boost::any Interpreter::visitVarDecl(const VarDecl *node) {
    printf("%s\n", node.toString().c_str());

    StringToken *token = (StringToken *)node.id.token;
    if (node.expr.token->type == TokenType::END) {
        this->global_.emplace(token->value, -999);
    } else {
        this->global_.emplace(token->value, evaluate(node.expr));
    }

    return 0;
}

boost::any Interpreter::visitType(const Type *node) {
    printf("%s\n", node.toString().c_str());

    return 0;
}

boost::any Interpreter::visitVar(const Var *node) {
    printf("%s\n", node.toString().c_str());

    StringToken *token = (StringToken *)node.token;
    return this->global_.at(token->value); // throws
}

boost::any Interpreter::visitNum(const Num *node) {
    printf("%s\n", node.toString().c_str());

    IntegerToken *token = (IntegerToken *)node.token;
    return token->value;
}

boost::any Interpreter::visitString(const String *node) {
    printf("%s\n", node.toString().c_str());
    return -27;
}

boost::any Interpreter::interpret() {
    printf("\nINTERPRETING...\n");

    evaluate(*this->root_);

    printf("\n%s \n", this->toString().c_str());

    return 0;
}
// end interpreter

} // namespace bluefin
