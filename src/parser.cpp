#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

Lexer::Lexer(std::string expr, std::stringstream &buffer)
    : expr_(std::move(expr)), buffer_(buffer) {
    this->tokenStart_ = 0;
    this->tokenLen_ = 1;
    this->line = 1;
}

// Checks if lexer has reached the end of expr
bool Lexer::hasMoreChars() {
    return this->tokenStart_ < this->expr_.length();
}

// Checks if current token hasd reached the end of expr
bool Lexer::tokenHasMoreChars() {
    return this->tokenStart_ + this->tokenLen_ < this->expr_.length();
}

// Returns the next char in the expression
char Lexer::peek() {
    if (tokenHasMoreChars()) {
        return this->expr_[this->tokenStart_ + this->tokenLen_];
    } else {
        return '\0';
    }
}

// Returns the next next char in the expression
char Lexer::peekNext() {
    if (this->tokenStart_ + this->tokenLen_ + 1 < this->expr_.length()) {
        return this->expr_[this->tokenStart_ + this->tokenLen_ + 1];
    } else {
        return '\0';
    }
}

// Peeks and increments length if character is a match
bool Lexer::nextMatches(char c) {
    if (this->peek() == c) {
        this->tokenLen_++;
        return true;
    } else {
        return false;
    }
}

std::string Lexer::getCurrentTokenString() {
    return this->expr_.substr(this->tokenStart_, this->tokenLen_);
}

Token Lexer::nextToken() {
    this->skipWhitespace();

    if (!hasMoreChars()) {
        return makeToken(TokenType::END);
    }

    char firstChar = this->expr_[this->tokenStart_];
    Token token;

    if (isdigit(firstChar)) {
        token = nextNumber();
    } else if (isalpha(firstChar)) {
        token = nextID();
    } else if (firstChar == '/' && this->peek() == '*') {
        // Block comment
        this->tokenStart_ += 2;
        this->skipComment();
        return this->nextToken();
    } else {
        // clang-format off
        switch (firstChar) {
            case '{': token = makeToken(TokenType::BSTR); break;
            case '}': token = makeToken(TokenType::BEND); break;
            case '(': token = makeToken(TokenType::PSTR); break;
            case ')': token = makeToken(TokenType::PEND); break;
            case ';': token = makeToken(TokenType::SEMI); break;
            case ',': token = makeToken(TokenType::COMMA); break;
            case '.': token = makeToken(TokenType::DOT); break;
            case '~': token = makeToken(TokenType::MUTABLE); break;
            case '?': token = makeToken(TokenType::NULLABLE); break;
            case '!': 
                token = nextMatches('=') ? makeToken(TokenType::NOT_EQ) : makeToken(TokenType::NOT); 
                break;
            case '=':
                token = nextMatches('=') ? makeToken(TokenType::EQ_EQ) : makeToken(TokenType::EQ); 
                break;
            case '>':
                token = nextMatches('=') ? makeToken(TokenType::GREATER_EQ) : makeToken(TokenType::GREATER); 
                break;
            case '<':
                token = nextMatches('=') ? makeToken(TokenType::LESS_EQ) : makeToken(TokenType::LESS); 
                break;
            case '+': token = makeToken(TokenType::PLUS); break;
            case '-': token = makeToken(TokenType::MINUS); break;
            case '*': token = makeToken(TokenType::MULT); break;
            case '/': token = handleForwardSlash(); break;
            case '%': token = makeToken(TokenType::REM); break;
            case '"': token = nextString(); break;
            case '\n': line++; break;
            default: 
                error(line, "Unexpected Token.", buffer_); 
                this->tokenStart_ += 1;
                return nextToken();
        }
        // clang-format on
    }

    printf(
        "next token %s %d %d\n",
        token.toString().c_str(),
        tokenStart_,
        tokenLen_);

    this->tokenStart_ += this->tokenLen_;
    this->tokenLen_ = 1;

    return token;
}

void Lexer::skipWhitespace() {
    while (hasMoreChars() && this->expr_[this->tokenStart_] == ' ')
        this->tokenStart_++;
}

Token Lexer::handleForwardSlash() {
    if (this->peek() == '/') {
        this->tokenLen_ = 2;
        return makeToken(TokenType::INT_DIV);
    } else {
        return makeToken(TokenType::DIV);
    }
}

void Lexer::skipComment() {
    while (hasMoreChars() && this->expr_[this->tokenStart_] != '*' &&
           this->peek() != '/')
        this->tokenStart_++;

    this->tokenStart_ += 2;
}

Token Lexer::nextNumber() {
    while (tokenHasMoreChars() && isdigit(peek()))
        this->tokenLen_++;

    if (tokenHasMoreChars() && peek() == '.' && isdigit(peekNext())) {
        this->tokenLen_++;

        while (tokenHasMoreChars() && isdigit(peek()))
            this->tokenLen_++;

        return makeToken(TokenType::DOUBLE_LITERAL, getCurrentTokenString());
    } else {
        return makeToken(TokenType::INTEGER_LITERAL, getCurrentTokenString());
    }
}

// ID: ([a-zA-Z]|_)*
bool Lexer::isNextCharID() {
    char nextChar;
    return tokenHasMoreChars() &&
        ((nextChar = peek()) == '_' || isalnum(nextChar));
}

Token Lexer::nextID() {
    while (isNextCharID())
        this->tokenLen_++;

    std::string tokenString = getCurrentTokenString();

    return RESERVED_KEYWORDS.count(tokenString)
        ? makeToken(RESERVED_KEYWORDS.at(tokenString), tokenString)
        : makeToken(TokenType::ID, tokenString);
}

Token Lexer::nextString() {
    while (peek() != '"' && tokenHasMoreChars()) {
        if (peek() == '\n')
            this->line++;
        this->tokenLen_++;
    }

    if (!tokenHasMoreChars()) {
        error(this->line, "String does not terminate.", buffer_);
    }

    std::string tokenString =
        this->expr_.substr(this->tokenStart_ + 1, this->tokenLen_ - 1);
    this->tokenLen_++;

    return makeToken(TokenType::STRING_LITERAL, tokenString);
}

Token Lexer::makeToken(TokenType type) {
    return makeToken(type, "");
}

Token Lexer::makeToken(TokenType type, std::string value) {
    return Token(type, value, line);
}
// end Lexer

Parser::Parser(Lexer lexer, std::stringstream &buffer)
    : lexer_(std::move(lexer)), buffer_(buffer) {
    this->currToken_ = this->lexer_.nextToken();

    printf("\nPARSING...\n");
}

void Parser::consume(TokenType type) {
    printf("Consume: %s\n", TokenTypeStrings[static_cast<int>(type)]);

    if (type != this->currToken_.type) {
        throw_error(
            "Parser consume failed on token" + this->currToken_.toString());
    }
    this->currToken_ = this->lexer_.nextToken();
}

// parse : program
AST *Parser::parse() {
    AST *node = this->program();

    if (this->currToken_.type != TokenType::END) {
        throw_error(
            "Parser expected END instead found " + this->currToken_.toString());
    }

    printf("parsing finished\n");
    return node;
}

// program = compound_statement
//         | program compound_statement
AST *Parser::program() {
    Program *node = new Program();
    Token op = this->currToken_;

    while (op.type == TokenType::BSTR) {
        node->blocks.push_back(*this->compound_statement());
        op = this->currToken_;
    }

    return node;
}

// compound_statement = "{" statement_list "}"
AST *Parser::compound_statement() {
    this->consume(TokenType::BSTR);
    std::vector<std::reference_wrapper<AST>> children = this->statement_list();
    this->consume(TokenType::BEND);

    Compound *root = new Compound();
    for (AST &child : children) {
        root->children.push_back(child);
    }

    return root;
}

// statement_list = statement
//                | statement statement_list
std::vector<std::reference_wrapper<AST>> Parser::statement_list() {
    AST *node = this->statement();
    std::vector<std::reference_wrapper<AST>> result = {*node};

    if (this->currToken_.type != TokenType::SEMI) {
        throw_error(
            "statement_list() statement in unexpected token" +
            this->currToken_.toString());
    }

    do {
        this->consume(TokenType::SEMI);
        result.push_back(*this->statement());
    } while (this->currToken_.type == TokenType::SEMI);

    if (this->currToken_.type != TokenType::BEND) {
        throw_error(
            "statement_list() ended in unexpected token" +
            this->currToken_.toString());
    }

    return result;
}

// statement = compound_statement ";"
//           | assignment_expression ";"
//           | white space
AST *Parser::statement() {
    switch (this->currToken_.type) {
        case TokenType::BSTR:
            return this->compound_statement();
        case TokenType::INTEGER:
        case TokenType::DOUBLE:
        case TokenType::STRING:
            return this->declaration();
        case TokenType::ID:
            return this->assignment_expr();
        default:
            return new NoOp();
    }
}

// additive_expression : multiplicative((+|-)multiplicative)*
AST *Parser::additive_expr() {
    AST *node = this->multiplicative_expr();
    Token op = this->currToken_;

    while (op.type == TokenType::PLUS || op.type == TokenType::MINUS) {
        printf("op     :%s\n", op.toString().c_str());
        if (op.type == TokenType::PLUS) {
            this->consume(TokenType::PLUS);
        } else {
            this->consume(TokenType::MINUS);
        }

        node = new BinOp(*node, op, *this->multiplicative_expr());
        op = this->currToken_;
    }

    return node;
}

// multiplicative_expression : unary_expression((*|/|%)unary_expression)*
AST *Parser::multiplicative_expr() {
    AST *node = this->unary_expr();
    Token op = this->currToken_;

    while (op.type == TokenType::MULT || op.type == TokenType::DIV ||
           op.type == TokenType::REM) {
        printf("op     :%s\n", op.toString().c_str());
        if (op.type == TokenType::MULT) {
            this->consume(TokenType::MULT);
        } else if (op.type == TokenType::DIV) {
            this->consume(TokenType::DIV);
        } else if (op.type == TokenType::INT_DIV) {
            this->consume(TokenType::INT_DIV);
        } else {
            this->consume(TokenType::REM);
        }
        node = new BinOp(*node, op, *this->unary_expr());
        op = this->currToken_;
    }

    return node;
}

// unary_expression = primary_expression
//                  | unary_operator primary_expression
AST *Parser::unary_expr() {
    Token op = this->currToken_;

    switch (op.type) {
        case TokenType::PLUS:
            this->consume(TokenType::PLUS);
            return new UnaryOp(*this->primary_expr(), op);
        case TokenType::MINUS:
            this->consume(TokenType::MINUS);
            return new UnaryOp(*this->primary_expr(), op);
        default:
            return this->primary_expr();
    }
}

// primary_expression = indentifier
//                    | constant
//                    | "(" expression ")"
AST *Parser::primary_expr() {
    Token expr = this->currToken_;
    printf("primary_expr :%s\n", expr.toString().c_str());

    switch (expr.type) {
        case TokenType::INTEGER_LITERAL:
            this->consume(TokenType::INTEGER_LITERAL);
            return new Num(expr);
        case TokenType::DOUBLE_LITERAL:
            this->consume(TokenType::DOUBLE_LITERAL);
            return new Num(expr);
        case TokenType::STRING_LITERAL:
            this->consume(TokenType::STRING_LITERAL);
            return new String(expr);
        case TokenType::PSTR: {
            this->consume(TokenType::PSTR);
            AST *node = additive_expr();
            this->consume(TokenType::PEND);
            return node;
        }
        case TokenType::ID:
            this->consume(TokenType::ID);
            return new Var(expr);
        default:
            throw_error(
                "Token type not primary expression: " + expr.toString());
    }
}

// assignment_expression = additive_expression
//                       | unary_expression assignment_operator
//                       assignment_expression
AST *Parser::assignment_expr() {
    // This currently is a hacky fix
    // need to find a way for it to parse int x = a+2;
    // i.e. an additive expression;
    Token lhs = this->currToken_;
    if (lhs.type == TokenType::INTEGER_LITERAL ||
        lhs.type == TokenType::DOUBLE_LITERAL) {
        return additive_expr();
    }

    AST *left = this->unary_expr();

    Token assignmentOp = this->currToken_;
    if (currToken_.type == TokenType::SEMI) {
        return left;
    }

    this->consume(TokenType::EQ);

    AST *right = this->additive_expr();
    return new Assign(*left, assignmentOp, *right);
}

// declaration = {type_specifier}+ identifier ("=" assignment_expression)? ";"
AST *Parser::declaration() {
    AST *type = this->type_spec();

    AST *id = new Var(this->currToken_);
    this->consume(TokenType::ID);

    if (this->currToken_.type == TokenType::EQ) {
        this->consume(TokenType::EQ);

        AST *rhs = this->assignment_expr();
        return new VarDecl(*type, *id, *rhs);
    } else {
        AST *null = new AST();
        return new VarDecl(*type, *id, *null);
    }
}

// type_specifier = int
//                | double
AST *Parser::type_spec() {
    Token type = this->currToken_;

    switch (type.type) {
        case TokenType::INTEGER:
            this->consume(TokenType::INTEGER);
            break;
        case TokenType::DOUBLE:
            this->consume(TokenType::DOUBLE);
            break;
        case TokenType::STRING:
            this->consume(TokenType::STRING);
            break;
        default:
            throw_error("Parser found invalid type: " + type.toString());
    }

    return new Type(type);
}

void Parser::throw_error(const std::string &message) {
    error(lexer_.line, message, buffer_);
}
// end parser

} // namespace bluefin
