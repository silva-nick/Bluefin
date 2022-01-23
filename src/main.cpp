#include "calc.hpp"

#include <iostream>
#include <string>

using namespace bluefin;

int main(int argc, char *argv[]) {
  std::cout << "> hello" << std::endl;
  std::cout << run("1+1") << std::endl;

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
    std::cout << run(input) << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
