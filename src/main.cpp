#include "bluefin.hpp"

#include <iostream>
#include <string>

using namespace bluefin;

void run_lexer(const std::string &input);
void run_parser(const std::string &input);

int main(int argc, char *argv[]) {
    std::cout << "> hello" << std::endl;
    std::cout << run("{{x=10*(1+1);variable=-x+3;};g_=10;}") << std::endl;

    while (true) {
        // Read
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        // Check for escape characters
        if (input == "q" || input == "quit") {
            break;
        } else if (input == "l" || input == "lexer") {
            std::cout << "> ";
            std::getline(std::cin, input);
            run_lexer(input);
        } else if (input == "p" || input == "parser") {
            std::cout << "filename: ";
            std::getline(std::cin, input);
            run_parser(input);
        } else {
            // Eval and print
            std::cout << run(input) << std::endl;
        }
    }

    std::cout << std::endl;

    return 0;
}

void run_lexer(const std::string &input) {
    Token token;
    Lexer lexer(input);
    while ((token = lexer.nextToken()).type != TokenType::END) {
        std::cout << "nextToken(): " << token.toString() << std::endl;
    }
    return;
}

void run_parser(const std::string &input) {
    Lexer lexer(input);
    Parser parser(lexer);

    AST *root = parser.parse();
}
