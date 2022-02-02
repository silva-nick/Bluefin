#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

int run(const std::string &expr) {
    Lexer lexer(expr);
    Parser parser(lexer);
    Interpreter interpreter(parser);
    return interpreter.interpret();
}

Lexer::Lexer(std::string expr) : expr_(std::move(expr)) {
    this->tokenStart_ = 0;
    this->tokenLen_ = 1;
}

// Checks if lexer has reached the end of expr
bool Lexer::hasMoreChars() {
    return this->tokenStart_ < this->expr_.length();
}

// Checks if current token hasd reached the end of expr
bool Lexer::tokenHasMoreChars() {
    return this->tokenStart_ + this->tokenLen_ < this->expr_.length();
}

char Lexer::peek() {
    if (tokenHasMoreChars()) {
        return this->expr_[this->tokenStart_ + this->tokenLen_];
    } else {
        return '\0';
    }
}

std::string Lexer::getCurrentTokenString() {
    return this->expr_.substr(this->tokenStart_, this->tokenLen_);
}

Token Lexer::nextToken() {
    printf("next token %s %d\n", expr_.c_str(), tokenStart_);
    // Skip white space
    while (hasMoreChars() && this->expr_[this->tokenStart_] == ' ')
        this->tokenStart_++;
    if (!hasMoreChars()) {
        return Token();
    }

    char firstChar = this->expr_[this->tokenStart_];
    Token token;

    if (isdigit(firstChar)) {
        token = nextInteger();
    } else if (isalpha(firstChar)) {
        token = nextID();
    } else {
        switch (firstChar) {
            case '=':
                token = Token(TokenType::ASSIGN, "=");
            case ';':
                token = Token(TokenType::SEMI, ";");
            case '+':
                token = Token(TokenType::PLUS, "+");
                break;
            case '-':
                token = Token(TokenType::MINUS, "-");
                break;
            case '*':
                token = Token(TokenType::MULT, "*");
                break;
            case '/':
                token = Token(TokenType::DIV, "/");
                break;
            case '%':
                token = Token(TokenType::REM, "%");
                break;
            case '(':
                token = Token(TokenType::PSTR, "(");
                break;
            case ')':
                token = Token(TokenType::PEND, ")");
                break;
            default:
                break;
        }
    }

    this->tokenStart_ += this->tokenLen_;
    this->tokenLen_ = 1;

    return token;
}

Token Lexer::nextInteger() {
    while (tokenHasMoreChars() &&
           isdigit(this->expr_[this->tokenStart_ + this->tokenLen_]))
        this->tokenLen_++;
    return Token(TokenType::INTEGER, getCurrentTokenString());
}

Token Lexer::nextID() {
    while (tokenHasMoreChars() &&
           isalnum(this->expr_[this->tokenStart_ + this->tokenLen_]))
        this->tokenLen_++;
    std::string tokenString = getCurrentTokenString();

    return RESERVED_KEYWORDS.count(tokenString)
        ? RESERVED_KEYWORDS.at(tokenString)
        : Token(TokenType::ID, getCurrentTokenString());
}
// end Lexer

Parser::Parser(Lexer lexer) : lexer_(std::move(lexer)) {
    this->currToken_ = this->lexer_.nextToken();
}

void Parser::consume(TokenType type) {
    assert(type == this->currToken_.type);
    this->currToken_ = this->lexer_.nextToken();
}

// parse : program
AST *Parser::parse() {
    AST *node = this->program();
    assert(this->currToken_.type == TokenType::END);
    
    return node;
}

// program = compound_statement
//         | program compound_statement
AST *Parser::program() {
    AST *node = this->compound_statement();
    Token op = this->currToken_;

    while (op.type == TokenType::BSTR) {
        node = this->compound_statement();
        op = this->currToken_;
    }

    printf("parsing finished\n\n");
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

    while (this->currToken_.type == TokenType::SEMI) {
        this->consume(TokenType::SEMI);
        result.push_back(*this->statement());
    }

    assert(this->currToken_.type != TokenType::ID);

    return result;
}

// statement = compound_statement ";"
//           | assignment_expression ";"
//           | white space
AST *Parser::statement() {
    switch (this->currToken_.type) {
        case TokenType::BSTR:
            return this->compound_statement();
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
            assert(0);
    }
}

// primary_expression = indentifier
//                    | constant
//                    | "(" expression ")"
AST *Parser::primary_expr() {
    Token expr = this->currToken_;
    printf("primary_expr :%s\n", expr.toString().c_str());

    switch (expr.type) {
        case TokenType::INTEGER:
            this->consume(TokenType::INTEGER);
            return new Num(expr);
        case TokenType::PSTR: {
            this->consume(TokenType::PSTR);
            AST *node = additive_expr();
            this->consume(TokenType::PEND);
            return node;
        }
        case TokenType::ID:
            this->consume(TokenType::ID);
            return new Var(this->currToken_);
        default:
            assert(0);
    }
}

// assignment_expression = additive_expression
//                       | unary_expression assignment_operator
//                       assignment_expression
AST *Parser::assignment_expr() {
    AST *left = this->unary_expr();
    Token assignmentOp = this->currToken_;
    this->consume(TokenType::ASSIGN);
    AST *right = this->additive_expr();
    return new Assign(*left, assignmentOp, *right);
}

// end parser

Interpreter::Interpreter(Parser parser) : parser_(std::move(parser)) {}

int Interpreter::visit(const AST &node) const {
    printf("visiting %d \n", static_cast<int>(node.type));
    switch (node.type) {
        case ASTType::BinOp:
            return visitBinOp(static_cast<const BinOp &>(node));
        case ASTType::Num:
            return visitNum(static_cast<const Num &>(node));
        case ASTType::UnaryOp:
            return visitUnaryOp(static_cast<const UnaryOp &>(node));
        default:
            assert(0);
    }
}

int Interpreter::visitBinOp(const BinOp &node) const {
    printf("binop %s \n", node.token.toString().c_str());
    switch (node.token.type) {
        case TokenType::PLUS:
            return this->visit(node.left) + this->visit(node.right);
            break;
        case TokenType::MINUS:
            return this->visit(node.left) - this->visit(node.right);
            break;
        case TokenType::MULT:
            return this->visit(node.left) * this->visit(node.right);
            break;
        case TokenType::DIV:
            return this->visit(node.left) / this->visit(node.right);
            break;
        case TokenType::REM:
            return this->visit(node.left) % this->visit(node.right);
            break;
        default:
            assert(0);
    }
}

int Interpreter::visitUnaryOp(const UnaryOp &node) const {
    switch (node.token.type) {
        case TokenType::PLUS:
            return +this->visit(node.child);
            break;
        case TokenType::MINUS:
            return -this->visit(node.child);
            break;
        default:
            assert(0);
    }
}

int Interpreter::visitNum(const Num &node) const {
    printf("leaf %s \n", node.token.toString().c_str());
    return std::stoi(node.token.value);
}

int Interpreter::interpret() {
    // search for new keyword
    AST *root = this->parser_.parse();

    printf("root node %s \n", root->token.toString().c_str());

    return this->visit(*root);
}
// end interpreter

} // namespace bluefin
