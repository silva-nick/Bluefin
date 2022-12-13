#include "Test.hpp"

namespace {

using namespace bluefin;

TEST(TypeChecking, NoDeclTest) {
    EXPECT_THAT(
        captureErrors("{a=10;}"),
        testing::HasSubstr("[line 1] Error: Variable doesn't exist: a"));

    // EXPECT_EQ(
    //     captureErrors("{int a=10; b = 1 + a;}"),
    //     "[line 1] Error: Variable doesn't exist: b");
}

TEST(TypeChecking, NanTypeTest) {
    EXPECT_THAT(
        captureErrors("{type a=10;}"),
        testing::HasSubstr(
            "[line 1] Error: Parser consume failed on token Token(ID, a)"));
}

} // namespace
