#include "calc.hpp"

namespace bluefin {
namespace {
bool isASCIILetter(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
bool isASCIIDigit(char c) {
  return (c >= '0' && c <= '9');
}
bool isASCIILetterOrDigit(char c) {
  return isASCIILetter(c) || isASCIIDigit(c);
}
bool isStrType(const std::string str, bool(*charType)(char)) {
  for (char c : str) {
    if (!charType(c)) {
      return false;
    }
  }
  return true;
}
} // namespace

Token::Token(TokenType type, char value) {
    this->type = type;
    this->value = value;
}

Token::Token() {
    this->type = TokenType::END;
    this->value = '\0';
}

std::string Token::toString() {
  return "Token(" + this->getTokenTypeString() + "," + this->value + ")";
}

std::string Token::getTokenTypeString() {
    return TokenTypeStrings[(int)this->type];
}

Interpreter::Interpreter(std::string expr) {
    this->expr = expr;
    this->tokenStart_ = 0;
    this->tokenEnd_ = 0;
    this->currToken_ = Token();
}

bool Interpreter::hasMoreTokens() {
    return this->tokenStart_ < expr.length();
}

void Interpreter::consume(TokenType type) {
  assert(type==this->currToken_.type);
  this->currToken_ = this->nextToken();
}

Token Interpreter::nextToken() {
    if (!hasMoreTokens()) {
        return Token();
    }

    char currChar = this->expr[tokenStart_];
    this->tokenStart_++;


    if (isASCIIDigit(currChar)) {
        return Token(TokenType::INTEGER, currChar);
    }

    if (currChar == '+') {
        return Token(TokenType::PLUS, currChar);
    }

    return Token();
}

int Interpreter::parse() {
  this->currToken_ = this->nextToken();
  Token lhs = this->currToken_;
  this->consume(TokenType::INTEGER);

  Token op = this->currToken_;
  this->consume(TokenType::PLUS);

  Token rhs = this->currToken_;
  this->consume(TokenType::INTEGER);

  assert(this->nextToken().type == TokenType::END);

  return rhs.value + lhs.value - '0' - '0';
}

} // namespace bluefin
