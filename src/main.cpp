#include "calc.hpp"

#include <iostream>
#include <string>

using namespace bluefin;

int test_calc(std::string expr) {
  Interpreter intr(expr);
  return intr.parse();
}

int main(int argc, char *argv[]) {
  std::cout << "> hello" << std::endl;

  while (true) {
    // Read
    std::cout << "> ";
    std::string input;
    std::cin >> input;

    // Eval and print
    if (input == "q" || input == "quit") {
      break;
    }
    std::cout << test_calc(input) << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
