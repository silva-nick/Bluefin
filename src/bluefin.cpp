#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

size_t ERROR_STATUS = 0;

int run(const std::string &expr) {
    std::stringstream buffer;
    int res = run(expr, buffer);
    std::cout << buffer.str() << std::endl;
    return res;
}

int run(const std::string &expr, std::stringstream &buffer) {
    Lexer lexer(expr, buffer);
    Parser parser(lexer, buffer);

    AST *root = parser.parse();
    if (ERROR_STATUS)
        return ERROR_STATUS;

    SymbolTableBuilder symbols(root, buffer);
    if (ERROR_STATUS)
        return ERROR_STATUS;

    Interpreter interpreter(root, buffer);
    interpreter.interpret();

    return ERROR_STATUS;
}

void error(size_t line, std::string message, std::stringstream &buffer) {
    buffer << "[line " << line << "] Error: " << message << std::endl;
    ERROR_STATUS = 1;
}

} // namespace bluefin