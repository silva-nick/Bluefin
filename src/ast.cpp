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

std::string Token::toString() {
  return "Token(" + this->getTokenTypeString() + "," + this->value + ")";
}

std::string Token::getTokenTypeString() {
  return TokenTypeStrings[(int)this->type];
}
// end Token

BinOp::BinOp(AST left, Token op, AST right) {
  this->type = ASTType::BinOp;
  this->left = left;
  this->token = op;
  this->right = right;
}
// end BinOp

Num::Num(Token token) {
  this->type = ASTType::Num;
  this->token = token;
}
// end Num

} // namespace bluefin
