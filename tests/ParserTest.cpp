#include "Test.hpp"

namespace {

TEST(PARSER, String) {
    EXPECT_THAT(captureErrors("{string x = \";}"), testing::HasSubstr("[line 1] Error: String does not terminate."));
}

TEST(PARSER, GoodToken) {
    EXPECT_EQ(captureErrors("{string x = \"test\";}"), "");
}

TEST(PARSER, BadToken) {
    EXPECT_THAT(captureErrors("{string x = #;}"), testing::HasSubstr("[line 1] Error: Unexpected Token."));
}

} // namespace
