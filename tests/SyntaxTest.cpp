#include "Test.hpp"

namespace {

using namespace bluefin;

TEST(Syntax, NoBrackets) {
    EXPECT_THAT(
        captureErrors("a=10;"),
        testing::HasSubstr(
            "[line 1] Error: Parser expected END instead found Token(ID, a)"));

    EXPECT_THAT(
        captureErrors("int a=10;}"),
        testing::HasSubstr(
            "[line 1] Error: Parser expected END instead found Token(INTEGER, int)"));

    EXPECT_THAT(
        captureErrors("{a=10;"),
        testing::HasSubstr(
            "[line 1] Error: statement_list() ended in unexpected token Token(END, )"));
}

TEST(Syntax, BadDecl) {
    EXPECT_THAT(
        captureErrors("{int a*b = 10;}"),
        testing::HasSubstr(
            "[line 1] Error: statement_list() statement in unexpected token Token(MULT, )"));

    EXPECT_THAT(
        captureErrors("{int a == 10;}"),
        testing::HasSubstr(
            "[line 1] Error: statement_list() statement in unexpected token Token(EQ_EQ, )"));

    EXPECT_THAT(
        captureErrors("{int a =   10}"),
        testing::HasSubstr(
            "[line 1] Error: statement_list() statement in unexpected token Token(BEND, )"));
}

} // namespace
