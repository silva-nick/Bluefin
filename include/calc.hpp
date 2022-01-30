#pragma once

#include <assert.h>
#include <string>
#include "ast.hpp"

namespace bluefin {

int run(const std::string &expr);

class Lexer {
 public:
  Lexer(const std::string &expr);
  Token nextToken();

 private:
  bool hasMoreChars();
  bool tokenHasMoreChars();
  std::string getCurrentTokenString();
  Token nextInteger();
  std::string expr_;
  size_t tokenStart_;
  size_t tokenLen_;
};

class Parser {
 public:
  Parser(Lexer lexer);
  AST *parse();

 private:
  void consume(TokenType type);
  AST *ADS();
  AST *MDR();
  AST *factor();
  Lexer &lexer_;
  Token currToken_;
};

class Interpreter {
 public:
  Interpreter(Parser &parser);
  int interpret();

 private:
  Parser &parser_;
  int visit(const AST &node) const;
  int visitBinOp(const BinOp &node) const;
  int visitNum(const Num &node) const;
};

} // namespace bluefin
