#pragma once

#include <assert.h>
#include <string>
#include "ast.hpp"

namespace bluefin {

class Lexer {
 public:
  Lexer();
  Lexer(std::string expr);
  Token nextToken();

 private:
  bool hasMoreChars();
  bool tokenHasMoreChars();
  std::string getCurrentTokenString();
  Token nextInteger();
  std::string expr;
  size_t tokenStart_;
  size_t tokenLen_;
};

class Parser {
 public:
  Parser();
  Parser(std::string expr);
  Parser(Lexer lexer);
  AST parse();

 private:
  void consume(TokenType type);
  AST MDR();
  AST factor();
  Lexer lexer_;
  Token currToken_;
};

class Interpreter {
 public:
  Interpreter(Parser parser);
  Interpreter(std::string expr);
  int interpret();

 private:
  Parser parser;
  int visit(AST &node);
  int visitBinOp(BinOp node);
  int visitNum(Num node);
};

} // namespace bluefin
