#pragma once

#include <string>

namespace bluefin {

// clang-format off
enum class TokenType : int {
    END = 0,
    // Brackets
    BSTR, BEND,
    PSTR, PEND,
    // Single char tokens
    SEMI, COMMA, DOT, MUTABLE, 
    NULLABLE, PLUS, MINUS,
    MULT, INT_DIV, DIV, REM,
    // Comparison operators
    NOT, NOT_EQ,
    EQ, EQ_EQ,
    GREATER, GREATER_EQ,
    LESS, LESS_EQ,
    // Literals
    ID,
    INTEGER, INTEGER_LITERAL,
    DOUBLE, DOUBLE_LITERAL,
    STRING, STRING_LITERAL,
    // KEYWORDS
    AND, CLASS, ELSE, FALSE, FUN, 
    FOR, IF, NONE, OR, RETURN, 
    SUPER, THIS, TRUE, WHILE
}; 

static constexpr const char *const TokenTypeStrings[45] = {
    "END",
    // Brackets
    "BSTR", "BEND",
    "PSTR", "PEND",
    // Single char tokens
    "SEMI", "COMMA", "DOT", "MUTABLE",
    "NULLABLE", "PLUS", "MINUS",
    "MULT", "INT_DIV", "DIV", "REM",
    // Comparison operators
    "NOT", "NOT_EQ",
    "EQ", "EQ_EQ",
    "GREATER", "GREATER_EQ",
    "LESS", "LESS_EQ",
    // Literals
    "ID",
    "INTEGER", "INTEGER_LITERAL",
    "DOUBLE", "DOUBLE_LITERAL",
    "STRING", "STRING_LITERAL",
    // KEYWORDS
    "AND", "CLASS", "ELSE", "FALSE", "FUN", 
    "FOR", "IF", "NONE", "OR", "RETURN",
    "SUPER", "THIS", "TRUE" , "WHILE"
};
// clang-format on

// Token definition
class Token {
   public:
    Token();
    Token(TokenType type, size_t line);

    TokenType type;
    size_t line;

    virtual std::string toString() const;

   protected:
    std::string getTokenTypeString() const;
};

class IntegerToken : public Token {
   public:
    IntegerToken();
    IntegerToken(TokenType type, size_t line, std::string value);

    int value;
    std::string toString() const;

   private:
};

class DoubleToken : public Token {
   public:
    DoubleToken();
    DoubleToken(TokenType type, size_t line, std::string value);

    double value;
    std::string toString() const;

   private:
};

class StringToken : public Token {
   public:
    StringToken();
    StringToken(TokenType type, size_t line, std::string value);

    std::string value;
    std::string toString() const;

   private:
};

} // namespace bluefin
