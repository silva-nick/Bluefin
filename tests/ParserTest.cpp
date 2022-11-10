#include "Test.hpp"

namespace {

TEST(Parser, String) {
    EXPECT_THAT(
        captureErrors("{string x = \";}"),
        testing::HasSubstr("[line 1] Error: String does not terminate."));
}

TEST(Parser, GoodToken) {
    EXPECT_EQ(captureErrors("{string x = \"test\";}"), "");
}

TEST(Parser, BadToken) {
    EXPECT_THAT(
        captureErrors("{string x = #;}"),
        testing::HasSubstr("[line 1] Error: Unexpected Token."));
}

TEST(Parser, MultiLine) {
    EXPECT_THAT(
        captureErrors("{int x = 1;\n int y = #;}"),
        testing::HasSubstr("[line 2] Error: Unexpected Token."));
}

} // namespace
