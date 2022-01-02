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

  if (isASCIIDigit(firstChar)) {
    return nextInteger();
  }

  if (firstChar == '+') {
    return Token(TokenType::PLUS, "+");
  }

  if (firstChar == '-') {
    return Token(TokenType::MINUS, "-");
  }

  if (firstChar == '*') {
    return Token(TokenType::MULT, "*");
  }

  if (firstChar == '/') {
    return Token(TokenType::DIV, "/");
  }

  return Token();
}

Token Interpreter::nextToken() {
  while (hasMoreChars() && this->expr[this->tokenStart_] == ' ')
    this->tokenStart_++;

  if (!hasMoreChars()) {

    return Token();
  }

  Token token = findToken();
  this->tokenStart_ += this->tokenLen_;
  this->tokenLen_ = 1;

  return token;
}

void Interpreter::consume(TokenType type) {
  assert(type == this->currToken_.type);
  this->currToken_ = this->nextToken();
}

int Interpreter::term() {
  Token integer = this->currToken_;

  printf("other %s\n", integer.toString().c_str());

  this->consume(TokenType::INTEGER);
  return std::stoi(integer.value);
}

int Interpreter::parse() {
  this->currToken_ = this->nextToken();
  Token integer = this->currToken_;
  printf("first %s\n", integer.toString().c_str());
  this->consume(TokenType::INTEGER);

  int res = std::stoi(integer.value);
  Token op = this->currToken_;

  while (op.type != TokenType::END) {
    printf("op %s\n", op.toString().c_str());
    switch (op.type) {
      case TokenType::PLUS:
        this->consume(TokenType::PLUS);
        res += this->term();
        break;
      case TokenType::MINUS:
        this->consume(TokenType::MINUS);
        res -= this->term();
        break;
      case TokenType::MULT:
        this->consume(TokenType::MULT);
        res *= this->term();
        break;
      case TokenType::DIV:
        this->consume(TokenType::DIV);
        res /= this->term();
        break;
      default:
        break;
    }

    op = this->currToken_;
  }

  assert(this->nextToken().type == TokenType::END);
  return res;
}

} // namespace bluefin
