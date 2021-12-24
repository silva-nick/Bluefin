#include "../include/calc.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(Calculator, AdditionTest) {
  Interpreter intr("1+1");
  EXPECT_EQ(intr.parse(), 2);

  Interpreter intrSpace("  1  + 1    ");
  EXPECT_EQ(intr.parse(), 2);

  Interpreter intrLong("667788+1970");
  EXPECT_EQ(intr.parse(), 669758);
}
} // namespace
