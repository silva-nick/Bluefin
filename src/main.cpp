#include "calc.hpp"

#include <string>

using namespace bluefin;

int test_calc(std::string expr) {
  Interpreter intr(expr);
  return intr.parse();
}

int main(int argc, char *argv[]) {
  printf("starting program execution: \n");
  printf("test_calc = %d\n", test_calc("1+1"));
  return 0;
}
