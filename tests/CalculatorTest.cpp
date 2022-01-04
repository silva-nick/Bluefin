#include "../include/calc.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(Calculator, AdditionTest) {
  Interpreter intr("1+1");
  EXPECT_EQ(intr.parse(), 2);

  Interpreter intrSpace("  1  + 1    ");
  EXPECT_EQ(intrSpace.parse(), 2);

  Interpreter intrLong("667788+1970");
  EXPECT_EQ(intrLong.parse(), 669758);
}

TEST(Calculator, OperatorTest) {
  Interpreter intrLong("10-10+10-10+10-10");
  EXPECT_EQ(intrLong.parse(), 0);

  Interpreter intrMult("10*10+1");
  EXPECT_EQ(intrMult.parse(), 101);

  Interpreter intrDiv("10/10+1");
  EXPECT_EQ(intrDiv.parse(), 2);
}

TEST(Calculator, MultDivTest) {
  Interpreter intr1("10+1*10");
  EXPECT_EQ(intr1.parse(), 20);

  Interpreter intr2("10-10*10-10+10/10");
  EXPECT_EQ(intr2.parse(), -99);
}

} // namespace
