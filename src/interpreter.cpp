#include "interpreter.hpp"
#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

Interpreter::Interpreter(std::stringstream &buffer) : buffer_(buffer) {}

void Interpreter::printVariables() const {
    std::cout << "Global vars: \n";

    for (const auto &x : this->global_) {
        if (x.second.type() == typeid(double)) {
            std::cout << x.first + ": " +
                    std::to_string(boost::any_cast<double>(x.second))
                      << std::endl;

        } else if (x.second.type() == typeid(int)) {
            std::cout << x.first + ": " +
                    std::to_string(boost::any_cast<int>(x.second))
                      << std::endl;

        } else if (x.second.type() == typeid(std::string)) {
            std::cout << x.first + ": " + boost::any_cast<std::string>(x.second)
                      << std::endl;
        }
    }
}

boost::any Interpreter::evaluate(const AST *node) {
    return node->accept((Visitor *)this);
}

bool Interpreter::isEqual(const boost::any &left, const boost::any &right)
    const {
    if (left.empty() && right.empty()) {
        return true;
    }

    if (left.type() != right.type()) {
        return false;
    }

    if (left.type() == typeid(std::string)) {
        return boost::any_cast<std::string>(left) ==
            boost::any_cast<std::string>(right);
    }

    if (left.type() == typeid(double)) {
        return boost::any_cast<double>(left) == boost::any_cast<double>(right);
    }

    if (left.type() == typeid(int)) {
        return boost::any_cast<int>(left) == boost::any_cast<int>(right);
    }

    return false;
}

void Interpreter::throw_error(Token *token, const std::string &message) const {
    error(token->line, message, buffer_);
}

boost::any Interpreter::visitProgram(const Program *node) {
    for (const AST *compound : node->compounds) {
        evaluate(compound);
    }
    return boost::any();
}

boost::any Interpreter::visitCompound(const Compound *node) {
    for (AST *statement : node->statements) {
        evaluate(statement);
    }
    return boost::any();
}

boost::any Interpreter::visitBinOp(const BinOp *node) {
    boost::any left = evaluate(node->left);
    boost::any right = evaluate(node->right);

    bool isInt = left.type() == typeid(int) && right.type() == typeid(int);

    double rawRes;

    switch (node->op->type) {
        case TokenType::PLUS:
            if (left.type() == typeid(double) &&
                right.type() == typeid(double)) {
                rawRes = boost::any_cast<double>(left) +
                    boost::any_cast<double>(right);
            }

            if (left.type() == typeid(std::string) &&
                right.type() == typeid(std::string)) {
                return boost::any_cast<std::string>(left) +
                    boost::any_cast<std::string>(right);
            }
            break;
        case TokenType::MINUS:
            rawRes =
                boost::any_cast<double>(left) - boost::any_cast<double>(right);
            break;
        case TokenType::MULT:
            rawRes =
                boost::any_cast<double>(left) * boost::any_cast<double>(right);
            break;
        case TokenType::DIV:
            rawRes =
                boost::any_cast<double>(left) / boost::any_cast<double>(right);
            break;
        case TokenType::INT_DIV:
            return boost::any_cast<int>(left) / boost::any_cast<int>(right);
            break;
        case TokenType::REM:
            return boost::any_cast<int>(left) % boost::any_cast<int>(right);
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
            return isEqual(right, left);
        case TokenType::NOT_EQ:
            return !isEqual(right, left);
        default:
            throw_error(node->op, "Invalid binary operand error.");
    }

    return isInt ? (int)rawRes : rawRes;
}

boost::any Interpreter::visitUnaryOp(const UnaryOp *node) {
    boost::any rawRes = evaluate(node->node);
    switch (node->op->type) {
        case TokenType::PLUS:
            if (rawRes.type() == typeid(double)) {
                return +boost::any_cast<double>(rawRes);
            } else {
                return +boost::any_cast<int>(rawRes);
            }
        case TokenType::MINUS:
            if (rawRes.type() == typeid(double)) {
                return -boost::any_cast<double>(rawRes);
            } else {
                return -boost::any_cast<int>(rawRes);
            }
        default:
            throw_error(node->op, "Invalid unary operand error.");
            return boost::any();
    }
}

boost::any Interpreter::visitAssign(const Assign *node) {
    StringToken *token = asStringToken(node->left);
    this->global_.emplace(token->value, evaluate(node->right));
    return boost::any();
}

boost::any Interpreter::visitVarDecl(const VarDecl *node) {
    StringToken *token = asStringToken(node->id);
    if (node->expr == NULL) {
        this->global_.emplace(token->value, NULL);
    } else {
        this->global_.emplace(token->value, evaluate(node->expr));
    }
    return boost::any();
}

boost::any Interpreter::visitType(const Type *node) {
    return boost::any();
}

boost::any Interpreter::visitVar(const Var *node) {
    StringToken *token = asStringToken((AST *)node);
    if (this->global_.count(token->value) == 0) {
        throw_error(token, "Variable has not been defined.");
        return boost::any();
    } else {
        return this->global_.at(token->value);
    }
}

boost::any Interpreter::visitNoOp(const NoOp *node) {
    return boost::any();
}

boost::any Interpreter::visitInteger(const Integer *node) {
    IntegerToken *token = (IntegerToken *)node->value;
    return token->value;
}

boost::any Interpreter::visitDouble(const Double *node) {
    DoubleToken *token = (DoubleToken *)node->value;
    return token->value;
}

boost::any Interpreter::visitString(const String *node) {
    StringToken *token = (StringToken *)node->value;
    return token->value;
}

void Interpreter::interpret(const AST *root) {
    printf("\nINTERPRETING...\n");
    evaluate(root);
}
// end interpreter

} // namespace bluefin
