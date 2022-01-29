#include "calc.hpp"

namespace bluefin {
namespace {
// Character types for lexeme matching
bool isASCIILetter(const char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
bool isASCIIDigit(const char c) {
  return (c >= '0' && c <= '9');
}
bool isASCIILetterOrDigit(const char c) {
  return isASCIILetter(c) || isASCIIDigit(c);
}
} // namespace

int run(const std::string &expr) {
  Lexer lexer(expr);
  Parser parser(lexer);
  Interpreter interpreter(parser);
  return interpreter.interpret();
}

Lexer::Lexer(const std::string &expr) : expr_(expr) {
  this->tokenStart_ = 0;
  this->tokenLen_ = 1;
}

// Checks if lexer has reached the end of expr
bool Lexer::hasMoreChars() {
  return this->tokenStart_ < this->expr_.length();
}

// Checks if current token hasd reached the end of expr
bool Lexer::tokenHasMoreChars() {
  return this->tokenStart_ + this->tokenLen_ < this->expr_.length();
}

std::string Lexer::getCurrentTokenString() {
  return this->expr_.substr(this->tokenStart_, this->tokenLen_);
}

Token Lexer::nextToken() {
  // Skip white space
  while (hasMoreChars() && this->expr_[this->tokenStart_] == ' ')
    this->tokenStart_++;
  if (!hasMoreChars()) {
    return Token();
  }

  char firstChar = this->expr_[this->tokenStart_];
  Token token;

  if (isASCIIDigit(firstChar)) {
    token = nextInteger();
  }

  switch (firstChar) {
    case '+':
      token = Token(TokenType::PLUS, "+");
      break;
    case '-':
      token = Token(TokenType::MINUS, "-");
      break;
    case '*':
      token = Token(TokenType::MULT, "*");
      break;
    case '/':
      token = Token(TokenType::DIV, "/");
      break;
    case '%':
      token = Token(TokenType::REM, "%");
      break;
    case '(':
      token = Token(TokenType::PSTR, "(");
      break;
    case ')':
      token = Token(TokenType::PEND, ")");
      break;
    default:
      break;
  }

  this->tokenStart_ += this->tokenLen_;
  this->tokenLen_ = 1;

  return token;
}

Token Lexer::nextInteger() {
  while (tokenHasMoreChars() &&
         isASCIIDigit(this->expr_[this->tokenStart_ + this->tokenLen_]))
    this->tokenLen_++;
  return Token(TokenType::INTEGER, getCurrentTokenString());
}
// end Lexer

Parser::Parser(Lexer lexer) : lexer_(lexer) {
  this->currToken_ = this->lexer_.nextToken();
}

void Parser::consume(TokenType type) {
  assert(type == this->currToken_.type);
  this->currToken_ = this->lexer_.nextToken();
}

// MDR : factor((*|/|%)factor)*
AST Parser::MDR() {
  AST node = this->factor();
  Token op = this->currToken_;

  while (op.type == TokenType::MULT || op.type == TokenType::DIV ||
         op.type == TokenType::REM) {
    printf("op     :%s\n", op.toString().c_str());
    if (op.type == TokenType::MULT) {
      this->consume(TokenType::MULT);
    } else if (op.type == TokenType::DIV) {
      this->consume(TokenType::DIV);
    } else {
      this->consume(TokenType::REM);
    }
    node = BinOp(node, op, this->factor());
    op = this->currToken_;
  }

  return node;
}

// factor : Integer | LPAREN parse RPAREN
AST Parser::factor() {
  Token factor = this->currToken_;
  printf("factor :%s\n", factor.toString().c_str());

  if (factor.type == TokenType::PSTR) {
    this->consume(TokenType::PSTR);
    AST node = parse();
    this->consume(TokenType::PEND);
    return node;
  } else {
    this->consume(TokenType::INTEGER);
    return Num(factor);
  }
}

// parse : MDR((+|-)MDR)*
AST Parser::parse() {
  AST node = this->MDR();
  Token op = this->currToken_;

  while (op.type == TokenType::PLUS || op.type == TokenType::MINUS) {
    printf("op     :%s\n", op.toString().c_str());
    if (op.type == TokenType::PLUS) {
      this->consume(TokenType::PLUS);
    } else {
      this->consume(TokenType::MINUS);
    }

    node = BinOp(node, op, this->MDR());
    op = this->currToken_;
  }

  printf("returning\n");
  return node;
}
// end parser

Interpreter::Interpreter(Parser &parser) : parser_(parser) {}

int Interpreter::visit(const AST &node) const {
  printf("visiting %d \n", static_cast<int>(node.type));
  switch (node.type) {
    case ASTType::BinOp:
      return visitBinOp(static_cast<const BinOp &>(node));
      break;
    case ASTType::Num:
      return visitNum(static_cast<const Num &>(node));
      break;
    default:
      assert(0);
  }
}

int Interpreter::visitBinOp(const BinOp &node) const {
  printf("binop %s \n", node.token.toString().c_str());
  switch (node.token.type) {
    case TokenType::PLUS:
      return this->visit(node.left) + this->visit(node.right);
      break;
    case TokenType::MINUS:
      return this->visit(node.left) - this->visit(node.right);
      break;
    case TokenType::MULT:
      return this->visit(node.left) * this->visit(node.right);
      break;
    case TokenType::DIV:
      return this->visit(node.left) / this->visit(node.right);
      break;
    case TokenType::REM:
      return this->visit(node.left) % this->visit(node.right);
      break;
    default:
      assert(0);
  }
}

int Interpreter::visitNum(const Num &node) const {
  printf("leaf %s \n", node.token.toString().c_str());
  return std::stoi(node.token.value);
}

int Interpreter::interpret() {
  AST root = this->parser_.parse();
  printf("root node %s \n", root.token.toString().c_str());
  printf(
      "left node %s \n", static_cast<BinOp &>(root).left.token.toString().c_str());

  return this->visit(root);
}
// end interpreter

} // namespace bluefin
