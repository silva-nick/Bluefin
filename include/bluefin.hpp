#pragma once

#include <iostream>
#include <string>
#include "ast.hpp"
#include "interpreter.hpp"
#include "parser.hpp"

namespace bluefin {

int run(const std::string &expr);
int error(unsigned line, std::string message);
extern unsigned ERROR_STATUS;

// TODO: make this compile time?
const std::unordered_map<std::string, Token> RESERVED_KEYWORDS = [] {
    std::unordered_map<std::string, Token> map = {
        {"int", Token(TokenType::INTEGER, "int", 0)},
        {"double", Token(TokenType::DOUBLE, "double", 0)}};
    return map;
}();

} // namespace bluefin