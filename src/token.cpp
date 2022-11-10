#include "token.hpp"

namespace bluefin {

Token::Token() : type(TokenType::END), line(0) {}

Token::Token(TokenType type, size_t line) : type(type), line(line) {}

std::string Token::toString() const {
    return "Token(" + this->getTokenTypeString() + ", )";
}

std::string Token::getTokenTypeString() const {
    return TokenTypeStrings[(int)this->type];
}

IntegerToken::IntegerToken() {
    this->type = TokenType::INTEGER_LITERAL;
    this->line = 0;
    this->value = 0;
}

IntegerToken::IntegerToken(TokenType type, size_t line, std::string value) {
    this->type = type;
    this->line = line;
    this->value = stoi(value);
}

std::string IntegerToken::toString() const {
    return "Token(" + getTokenTypeString() + ", " + std::to_string(value) + ")";
}

DoubleToken::DoubleToken() {
    this->type = TokenType::DOUBLE_LITERAL;
    this->line = 0;
    this->value = 0;
}

DoubleToken::DoubleToken(TokenType type, size_t line, std::string value) {
    this->type = type;
    this->line = line;
    this->value = stod(value);
}

std::string DoubleToken::toString() const {
    return "Token(" + getTokenTypeString() + ", " + std::to_string(value) + ")";
}

StringToken::StringToken() {
    this->type = TokenType::STRING_LITERAL;
    this->line = 0;
    this->value = "";
}

StringToken::StringToken(TokenType type, size_t line, std::string value) {
    this->type = type;
    this->line = line;
    this->value = value;
}

std::string StringToken::toString() const {
    return "Token(" + getTokenTypeString() + ", " + value + ")";
}

} // namespace bluefin
