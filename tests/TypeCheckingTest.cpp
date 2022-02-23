#include "../include/bluefin.hpp"

#include "gtest/gtest.h"

namespace {

using namespace bluefin;

TEST(TypeChecking, NoDeclTest) {
    EXPECT_THROW(run("{a=10;}"), std::out_of_range*);

    EXPECT_THROW(run("{int a=10; b = 1 + a;}"), std::out_of_range*);
}

TEST(TypeChecking, NanTypeTest) {
    EXPECT_THROW(run("{type a=10;}"), std::invalid_argument*);
}

} // namespace
