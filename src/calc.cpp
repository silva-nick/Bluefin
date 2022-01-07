#include "calc.hpp"

namespace bluefin {
namespace {
// Character types for lexeme matching
bool isASCIILetter(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
bool isASCIIDigit(char c) {
  return (c >= '0' && c <= '9');
}
bool isASCIILetterOrDigit(char c) {
  return isASCIILetter(c) || isASCIIDigit(c);
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
// end Token

Lexer::Lexer() {
  this->expr = "";
  this->tokenStart_ = 0;
  this->tokenLen_ = 1;
}

Lexer::Lexer(std::string expr) {
  this->expr = expr;
  this->tokenStart_ = 0;
  this->tokenLen_ = 1;
}

// Checks if lexer has reached the end of expr
bool Lexer::hasMoreChars() {
  return this->tokenStart_ < this->expr.length();
}

// Checks if current token hasd reached the end of expr
bool Lexer::tokenHasMoreChars() {
  return this->tokenStart_ + this->tokenLen_ < this->expr.length();
}

std::string Lexer::getCurrentTokenString() {
  return this->expr.substr(this->tokenStart_, this->tokenLen_);
}

Token Lexer::nextToken() {
  // Skip white space
  while (hasMoreChars() && this->expr[this->tokenStart_] == ' ')
    this->tokenStart_++;

  if (!hasMoreChars()) {
    return Token();
  }

  char firstChar = this->expr[this->tokenStart_];
  Token token;

  if (isASCIIDigit(firstChar)) {
    token = nextInteger();
  }
  if (firstChar == '+') {
    token = Token(TokenType::PLUS, "+");
  }
  if (firstChar == '-') {
    token = Token(TokenType::MINUS, "-");
  }
  if (firstChar == '*') {
    token = Token(TokenType::MULT, "*");
  }
  if (firstChar == '/') {
    token = Token(TokenType::DIV, "/");
  }

  this->tokenStart_ += this->tokenLen_;
  this->tokenLen_ = 1;

  return token;
}

Token Lexer::nextInteger() {
  while (tokenHasMoreChars() &&
         isASCIIDigit(this->expr[this->tokenStart_ + this->tokenLen_]))
    this->tokenLen_++;
  return Token(TokenType::INTEGER, getCurrentTokenString());
}
// end Lexer

Interpreter::Interpreter(std::string expr) {
  this->lexer_ = Lexer(expr);
  this->currToken_ = this->lexer_.nextToken();
}

Interpreter::Interpreter(Lexer lexer) {
  this->lexer_ = lexer;
  this->currToken_ = this->lexer_.nextToken();
}

void Interpreter::consume(TokenType type) {
  assert(type == this->currToken_.type);
  this->currToken_ = this->lexer_.nextToken();
}

// MDR : factor((*|/|%)factor)*
int Interpreter::MDR() {
  int result = this->factor();
  Token op = this->currToken_;

  while (op.type == TokenType::MULT || op.type == TokenType::DIV ||
         op.type == TokenType::REM) {
    if (op.type == TokenType::MULT) {
      this->consume(TokenType::MULT);
      result *= this->factor();
    } else if (op.type == TokenType::DIV) {
      this->consume(TokenType::DIV);
      result /= this->factor();
    } else {
      this->consume(TokenType::REM);
      result %= this->factor();
    }
    op = this->currToken_;
  }

  return result;
}

// factor : Integer
int Interpreter::factor() {
  Token integer = this->currToken_;

  printf("factor : %s\n", integer.toString().c_str());

  this->consume(TokenType::INTEGER);
  return std::stoi(integer.value);
}

// parse : MDR((+|-)MDR)*
int Interpreter::parse() {
  int result = this->MDR();
  Token op = this->currToken_;

  while (op.type != TokenType::END) {
    printf("op %s\n", op.toString().c_str());
    if (op.type == TokenType::PLUS) {
      this->consume(TokenType::PLUS);
      result += this->MDR();
    } else {
      this->consume(TokenType::MINUS);
      result -= this->MDR();
    }
    op = this->currToken_;
  }

  assert(this->lexer_.nextToken().type == TokenType::END);
  return result;
}

} // namespace bluefin
