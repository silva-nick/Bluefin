#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

int run(const std::string &expr) {
    Lexer lexer(expr);
    Parser parser(lexer);
    AST *root = parser.parse();
    SymbolTableBuilder symbols(root);
    Interpreter interpreter(root);
    return interpreter.interpret();
}

void error() {
    System.err.println(
        "[line " + line + "] Error" + where + ": " + message);
    ERROR_STATUS = 1;
}

} // namespace bluefin