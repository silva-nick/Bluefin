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
    SEMI, COMMA, DOT, PLUS, MINUS,
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
    FOR, IF, NIL, OR, RETURN, 
    SUPER, THIS, TRUE, WHILE,
};
static constexpr const char *const TokenTypeStrings[43] = {
    "END",
    // Brackets
    "BSTR", "BEND",
    "PSTR", "PEND",
    // Single char tokens
    "SEMI", "COMMA", "DOT", "PLUS", "MINUS",
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
    "FOR", "IF", "NIL", "OR", "RETURN",
    "SUPER", "THIS", "TRUE" , "WHILE",
};
// clang-format on

// Token definition
class Token {
   public:
    Token();
    Token(TokenType type, std::string value, unsigned line);
    TokenType type;
    std::string value;
    unsigned line;
    // literal object;
    std::string toString() const;

   private:
    std::string getTokenTypeString() const;
};

} // namespace bluefin
