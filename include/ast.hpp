#pragma once

#include <string>

namespace bluefin {

enum class TokenType : int {
  END = 0,
  PSTR,
  PEND,
  INTEGER,
  PLUS,
  MINUS,
  MULT,
  DIV,
  REM
};
static constexpr const char *const TokenTypeStrings[9] =
    {"END", "PSTR", "PEND", "INTEGER", "PLUS", "MINUS", "MULT", "DIV", "REM"};

// Token definition
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

enum class ASTType : int { BinOp = 0, Num };

// Tree node parent class
class AST {
 public:
  Token token;
  ASTType type;

 private:
};

// Binary operators
class BinOp : public AST {
 public:
  BinOp(AST left, Token op, AST right);
  AST left;
  AST right;

 private:
};

// Numbers / terminals
class Num : public AST {
 public:
  Num(Token token);

 private:
};

} // namespace bluefin
