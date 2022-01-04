#pragma once

#include <assert.h>
#include <string>

namespace bluefin {

enum class TokenType : int { END = 0, INTEGER, PLUS, MINUS, MULT, DIV };
static constexpr const char *const TokenTypeStrings[6] =
    {"END", "INTEGER", "PLUS", "MINUS", "MULT", "DIV"};

class Token {
 public:
  Token(TokenType type, std::string value);
  Token();
  TokenType type;
  std::string value;
  std::string toString();

 private:
  std::string getTokenTypeString();
};

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

class Interpreter {
 public:
  Interpreter(std::string expr);
  Interpreter(Lexer lexer);
  int parse();

 private:
  void consume(TokenType type);
  int factor();
  Lexer lexer_;
  Token currToken_;
};

} // namespace bluefin
