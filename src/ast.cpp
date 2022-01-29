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

AST::AST(Token value, ASTType nodeType) : token(std::move(value)), type(nodeType) {}
// end AST

BinOp::BinOp(AST leftVal,  Token op,  AST rightVal)
    : left(std::move(leftVal)), right(std::move(rightVal)), AST(op, ASTType::BinOp) {}
// end BinOp

Num::Num(Token token) : AST(token, ASTType::Num) {}
// end Num

} // namespace bluefin
