#include "Test.hpp"

namespace {

using namespace bluefin;

TEST(TypeChecking, NoDeclTest) {
    EXPECT_THROW(run("{a=10;}"), std::out_of_range);

    EXPECT_THROW(run("{int a=10; b = 1 + a;}"), std::out_of_range);
}

TEST(TypeChecking, NanTypeTest) {
    EXPECT_THAT(
        captureErrors("{type a=10;}"),
        testing::HasSubstr(
            "[line 1] Error: Parser consume failed on tokenToken(ID,a)"));
}

} // namespace
