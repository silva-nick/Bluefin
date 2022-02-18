#include "ast.hpp"

namespace bluefin {

Token::Token(TokenType type, std::string value) {
    this->type = type;
    this->value = value;
}

Token::Token() {
    this->type = TokenType::END;
    this->value = "";
}

std::string Token::toString() const {
    return "Token(" + this->getTokenTypeString() + "," + this->value + ")";
}

std::string Token::getTokenTypeString() const {
    return TokenTypeStrings[(int)this->type];
}
// end Token

AST::AST() {}

AST::AST(Token value, ASTType nodeType)
    : token(std::move(value)), type(nodeType) {}

std::string AST::toString() const {
    return "token:" + this->token.toString() +
        ", type:" + ASTTypeStrings[static_cast<int>(this->type)];
}
// end AST

Program::Program() : AST(Token(), ASTType::Program) {}

std::string Program::toString() const {
    std::string out;
    for (AST &node : this->blocks) {
        out.append(node.toString());
    }
    return out;
}
// end Program

Compound::Compound() : AST(Token(), ASTType::Compound) {}

std::string Compound::toString() const {
    std::string out;
    for (AST &node : this->children) {
        out.append(node.toString());
    }
    return out;
}
// end Compound

// Assignment
Assign::Assign(AST &leftVal, Token op, AST &rightVal)
    : left(leftVal), right(rightVal), AST(op, ASTType::Assign) {}

std::string Assign::toString() const {
    return AST::toString() + ", left:" + this->left.token.toString() +
        ", right:" + this->right.token.toString();
}
// end Assign

BinOp::BinOp(AST &leftVal, Token op, AST &rightVal)
    : left(leftVal), right(rightVal), AST(op, ASTType::BinOp) {}

std::string BinOp::toString() const {
    return AST::toString() + ", left:" + this->left.token.toString() +
        ", right:" + this->right.token.toString();
}
// end BinOp

UnaryOp::UnaryOp(AST &node, Token op)
    : child(node), AST(op, ASTType::UnaryOp) {}

std::string UnaryOp::toString() const {
    return AST::toString() + ", child: " + this->child.toString();
}
// end UnaryOp

NoOp::NoOp() : AST(Token(), ASTType::NoOp) {}
// end NoOp

VarDecl::VarDecl(AST &typeNode, AST &varNode, AST &rhsNode)
    : type(typeNode), id(varNode), expr(rhsNode), AST(Token(), ASTType::VarDecl) {}

std::string VarDecl::toString() const {
    return AST::toString() + ", type_val: " + this->type.toString() +
        ", id: " + this->id.toString() + ", expr: " + this->expr.toString();
}
// end VarDecl

Type::Type(Token token) : AST(token, ASTType::Type) {}

std::string Type::toString() const {
    return AST::toString();
}
// end Type

Var::Var(Token token) : AST(token, ASTType::Var) {}

std::string Var::toString() const {
    return AST::toString();
}
// end Var

Num::Num(Token token) : AST(token, ASTType::Num) {}

std::string Num::toString() const {
    return AST::toString();
}
// end Num

} // namespace bluefin
