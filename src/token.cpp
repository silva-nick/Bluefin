#include "token.hpp"

namespace bluefin {

Token::Token() {
    this->type = TokenType::END;
    this->value = "";
    this->line = 0;
}

Token::Token(TokenType type, std::string value, size_t line) {
    this->type = type;
    this->value = value;
    this->line = line;
}

std::string Token::toString() const {
    return "Token(" + this->getTokenTypeString() + "," + this->value + ")";
}

std::string Token::getTokenTypeString() const {
    return TokenTypeStrings[(int)this->type];
}

} // namespace bluefin
