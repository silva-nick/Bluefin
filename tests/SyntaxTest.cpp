#include "../include/bluefin.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(Syntax, NoBrackets) {
    EXPECT_THROW(run("a=10;"), std::invalid_argument);

    EXPECT_THROW(run("int a=10;}"), std::invalid_argument);

    EXPECT_THROW(run("{int a=10;"), std::invalid_argument *);
}

TEST(Syntax, BadDecl) {
    EXPECT_THROW(run("{int a*b = 10;}"), std::invalid_argument *);

    EXPECT_THROW(run("{int a12=10;}"), std::invalid_argument *);

    EXPECT_THROW(run("{int a==10;}"), std::invalid_argument *);

    EXPECT_THROW(run("{int a =   10}"), std::invalid_argument *);
}

} // namespace
