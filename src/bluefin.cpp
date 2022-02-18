#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

int run(const std::string &expr) {
    Lexer lexer(expr);
    Parser parser(lexer);
    Interpreter interpreter(parser.parse());
    return interpreter.interpret();
}

} // namespace bluefin