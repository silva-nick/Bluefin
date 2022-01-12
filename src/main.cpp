#include "calc.hpp"

#include <iostream>
#include <string>

using namespace bluefin;

int test_calc(std::string expr) {
  Lexer lexer(expr);
  Parser parser(lexer);
  Interpreter intr(parser);
  return intr.interpret();
}

int main(int argc, char *argv[]) {
  std::cout << "> hello" << std::endl;
  std::cout << "tst: " << test_calc("1+1") << std::endl;

  while (true) {
    // Read
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);

    // Check for escape characters
    if (input == "q" || input == "quit") {
      break;
    }

    // Eval and print
    std::cout << test_calc(input) << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
