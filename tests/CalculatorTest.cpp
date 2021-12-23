#include "../include/calc.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(Calculator, AdditionTest) {
  Interpreter intr("1+1");
  EXPECT_EQ(intr.parse(), 2);
}
} // namespace
