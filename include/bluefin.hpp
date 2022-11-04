#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "ast.hpp"
#include "interpreter.hpp"
#include "parser.hpp"

namespace bluefin {

int run(const std::string &expr);
int run(const std::string &expr, std::stringstream &buffer);
void error(size_t line, std::string message, std::stringstream &buffer);
extern size_t ERROR_STATUS;

} // namespace bluefin
