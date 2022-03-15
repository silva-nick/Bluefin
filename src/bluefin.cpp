#include "bluefin.hpp"

namespace bluefin {

int run(const std::string &expr) {
    Lexer lexer(expr);
    Parser parser(lexer);
    AST *root = parser.parse();
    SymbolTableBuilder symbols(root);
    Interpreter interpreter(root, symbols.getSymbols());
    return interpreter.interpret();
}

} // namespace bluefin