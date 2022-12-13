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
    if (ERROR_STATUS) {
        return ERROR_STATUS;
    }

    PrintVisitor printer = PrintVisitor();
    printer.print(root);

    SymbolTableBuilder symbols(buffer);
    symbols.build(root);
    if (ERROR_STATUS) {
        return ERROR_STATUS;
    }

    Interpreter interpreter(buffer);
    interpreter.interpret(root);
    interpreter.printVariables();

    return ERROR_STATUS;
}

void error(size_t line, std::string message, std::stringstream &buffer) {
    buffer << "[line " << line << "] Error: " << message << std::endl;
    ERROR_STATUS = 1;
}

} // namespace bluefin