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
bool isStrType(const std::string str, bool (*charType)(char)) {
  for (char c : str) {
    if (!charType(c)) {
      return false;
    }
  }
  return true;
}
} // namespace

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

Interpreter::Interpreter(std::string expr) {
  this->expr = expr;
  this->tokenStart_ = 0;
  this->tokenLen_ = 1;
  this->currToken_ = Token();
}

void Interpreter::consume(TokenType type) {
  assert(type == this->currToken_.type);
  this->currToken_ = this->nextToken();
}

Token Interpreter::nextInteger() {
  while (tokenHasMoreChars() &&
         isASCIIDigit(this->expr[this->tokenStart_ + this->tokenLen_]))
    this->tokenLen_++;
  return Token(TokenType::INTEGER, getCurrentTokenString());
}

bool Interpreter::hasMoreChars() {
  return this->tokenStart_ < this->expr.length();
}

bool Interpreter::tokenHasMoreChars() {
  return this->tokenStart_ + this->tokenLen_ < this->expr.length();
}

std::string Interpreter::getCurrentTokenString() {
  return this->expr.substr(this->tokenStart_, this->tokenLen_);
}

Token Interpreter::findToken() {
  char firstChar = this->expr[this->tokenStart_];

  if (firstChar == ' ') {
    return nextToken();
  }

  if (isASCIIDigit(firstChar)) {
    return nextInteger();
  }

  if (firstChar == '+') {
    return Token(TokenType::PLUS, "+");
  }

  if (firstChar == '-') {
    return Token(TokenType::MINUS, "-");
  }

  return Token();
}

Token Interpreter::nextToken() {
  if (!hasMoreChars()) {
    return Token();
  }

  Token token = findToken();
  this->tokenStart_ += this->tokenLen_;
  this->tokenLen_ = 1;

  return token;
}

int Interpreter::parse() {
  this->currToken_ = this->nextToken();
  Token lhs = this->currToken_;
  printf("lhs %s\n", lhs.toString().c_str());
  this->consume(TokenType::INTEGER);

  Token op = this->currToken_;
  printf("op %s\n", op.toString().c_str());
  bool plus = true;
  if (op.type == TokenType::PLUS) {
    this->consume(TokenType::PLUS);
  } else {
    this->consume(TokenType::MINUS);
    plus = false;
  }

  Token rhs = this->currToken_;
  printf("rhs %s\n", rhs.toString().c_str());
  this->consume(TokenType::INTEGER);

  assert(this->nextToken().type == TokenType::END);

  return std::stoi(lhs.value) + std::stoi(rhs.value) * (plus ? 1 : -1);
}

} // namespace bluefin
