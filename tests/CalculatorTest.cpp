#include "../include/calc.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(Calculator, AdditionTest) {
  Interpreter intr("1+1");
  EXPECT_EQ(intr.interpret(), 2);

  Interpreter intrSpace("  1  + 1    ");
  EXPECT_EQ(intrSpace.interpret(), 2);

  Interpreter intrLong("667788+1970");
  EXPECT_EQ(intrLong.interpret(), 669758);
}

TEST(Calculator, OperatorTest) {
  Interpreter intrLong("10-10+10-10+10-10");
  EXPECT_EQ(intrLong.interpret(), 0);

  Interpreter intrMult("10*10+1");
  EXPECT_EQ(intrMult.interpret(), 101);

  Interpreter intrDiv("10/10+1");
  EXPECT_EQ(intrDiv.interpret(), 2);
}

TEST(Calculator, MultDivTest) {
  Interpreter intr1("10+1*10");
  EXPECT_EQ(intr1.interpret(), 20);

  Interpreter intr2("10-10*10-10+10/10");
  EXPECT_EQ(intr2.interpret(), -99);

  Interpreter intr3("5%2+3");
  EXPECT_EQ(intr3.interpret(), 4);
}

TEST(Calculator, ParenTest) {
  Interpreter intr1("(1+1)*10");
  EXPECT_EQ(intr1.interpret(), 20);

  Interpreter intr2("7 + 3 * (10 / (12 / (3 + 1) - 1))");
  EXPECT_EQ(intr2.interpret(), 22);

  Interpreter intr3("5%(2+3)");
  EXPECT_EQ(intr3.interpret(), 0);
}


} // namespace
