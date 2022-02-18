#include "bluefin.hpp"

#include <iostream>
#include <string>

using namespace bluefin;

void run_lexer(const std::string &input);
void run_parser(const std::string &input);

int main(int argc, char *argv[]) {
    std::cout << "> hello" << std::endl;
    std::cout << run("{double g_=0.1;/*comment*/}") << std::endl;

    while (true) {
        // Read
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        // Check for escape characters
        if (input == "q" || input == "quit") {
            break;
        } else {
            // Eval and print
            std::cout << run(input) << std::endl;
        }
    }

    std::cout << std::endl;

    return 0;
}