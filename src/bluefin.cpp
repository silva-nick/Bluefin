#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

size_t ERROR_STATUS = 0;

int run(const std::string &expr) {
    Lexer lexer(expr);
    Parser parser(lexer);
    AST *root = parser.parse();
    SymbolTableBuilder symbols(root);
    Interpreter interpreter(root);
    return interpreter.interpret();
}

void error(size_t line, std::string message) {
    std::cerr << "[line " << line << "] Error: " << message << std::endl;
    ERROR_STATUS = 1;
}

} // namespace bluefin