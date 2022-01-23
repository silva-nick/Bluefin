#include "../include/calc.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(Calculator, AdditionTest) {
  EXPECT_EQ(run("1+1"), 2);
  
  EXPECT_EQ(run("  1  + 1    "), 2);

  EXPECT_EQ(run("667788+1970"), 669758);
}

TEST(Calculator, OperatorTest) {
  EXPECT_EQ(run("10-10+10-10+10-10"), 0);

  EXPECT_EQ(run("10*10+1"), 101);

  EXPECT_EQ(run("10/10+1"), 2);
}

TEST(Calculator, MultDivTest) {
  EXPECT_EQ(run("10+1*10"), 20);

  EXPECT_EQ(run("10-10*10-10+10/10"), -99);

  EXPECT_EQ(run("5%2+3"), 4);
}

TEST(Calculator, ParenTest) {
  EXPECT_EQ(run("(1+1)*10"), 20);

  EXPECT_EQ(run("7 + 3 * (10 / (12 / (3 + 1) - 1))"), 22);

  EXPECT_EQ(run("5%(2+3)"), 0);
}

} // namespace
