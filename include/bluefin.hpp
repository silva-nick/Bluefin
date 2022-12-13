#pragma once

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "ast.hpp"
#include "interpreter.hpp"
#include "parser.hpp"
#include "printvisitor.hpp"
#include "symboltable.hpp"

namespace bluefin {

int run(const std::string &expr);
int run(const std::string &expr, std::stringstream &buffer);
void error(size_t line, std::string message, std::stringstream &buffer);
extern size_t ERROR_STATUS;

} // namespace bluefin
