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

AST::AST(Token value, ASTType nodeType)
    : token(std::move(value)), type(nodeType) {}

std::string AST::toString() const {
    return "token:" + this->token.toString() +
        ", type:" + ASTTypeStrings[static_cast<int>(this->type)];
}
// end AST

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

Num::Num(Token token) : AST(token, ASTType::Num) {}

std::string Num::toString() const {
    return AST::toString();
}
// end Num

} // namespace bluefin
