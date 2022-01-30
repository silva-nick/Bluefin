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
  std::string toString() const;

 private:
  std::string getTokenTypeString() const;
};

enum class ASTType : int { BinOp = 0, Num };
static constexpr const char *const ASTTypeStrings[2] =
    {"BinOp", "Num"};


// Tree node parent class
class AST {
 public:
  AST(Token value, ASTType nodeType);

  Token token;
  ASTType type;

  virtual std::string toString() const;

 private:
};

// Binary operators
class BinOp : public AST {
 public:
  BinOp(AST &left, Token op, AST &right);

  AST &left;
  AST &right;

  std::string toString() const;

 private:
};

// Numbers / terminals
class Num : public AST {
 public:
  Num(Token token);

  std::string toString() const;

 private:
};

} // namespace bluefin
