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

// Returns the next char in the expression
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
    this->skipWhitespace();

    if (!hasMoreChars()) {
        return Token();
    }

    char firstChar = this->expr_[this->tokenStart_];
    Token token;

    if (isdigit(firstChar)) {
        token = nextNumber();
    } else if (isalpha(firstChar)) {
        token = nextID();
    } else if (firstChar == '/' && this->peek() == '*') {
        this->tokenStart_ += 2;
        this->skipComment();
        return this->nextToken();
    } else {
        switch (firstChar) {
            case '=':
                token = Token(TokenType::ASSIGN, "=");
                break;
            case ';':
                token = Token(TokenType::SEMI, ";");
                break;
            case '{':
                token = Token(TokenType::BSTR, "{");
                break;
            case '}':
                token = Token(TokenType::BEND, "}");
                break;
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
                if (this->peek() == '/')
                    token = Token(TokenType::INT_DIV, "//");
                else
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

void Lexer::skipComment() {
    while (hasMoreChars() && this->expr_[this->tokenStart_] != '*' &&
           this->peek() != '/')
        this->tokenStart_++;

    this->tokenStart_ += 2;
}

Token Lexer::nextNumber() {
    while (tokenHasMoreChars() &&
           isdigit(this->expr_[this->tokenStart_ + this->tokenLen_]))
        this->tokenLen_++;

    if (tokenHasMoreChars() &&
        this->expr_[this->tokenStart_ + this->tokenLen_++] == '.') {
        while (tokenHasMoreChars() &&
               isdigit(this->expr_[this->tokenStart_ + this->tokenLen_]))
            this->tokenLen_++;

        return Token(TokenType::DOUBLE_CONST, getCurrentTokenString());
    } else {
        return Token(TokenType::INTEGER_CONST, getCurrentTokenString());
    }
}

// ID: ([a-zA-Z]|_)*
bool Lexer::isNextCharID() {
    char nextChar;
    return tokenHasMoreChars() &&
        ((nextChar = this->expr_[this->tokenStart_ + this->tokenLen_]) == '_' ||
         isalnum(nextChar));
}

Token Lexer::nextID() {
    while (isNextCharID())
        this->tokenLen_++;

    std::string tokenString = getCurrentTokenString();

    return RESERVED_KEYWORDS.count(tokenString)
        ? RESERVED_KEYWORDS.at(tokenString)
        : Token(TokenType::ID, getCurrentTokenString());
}
// end Lexer

Parser::Parser(Lexer lexer) : lexer_(std::move(lexer)) {
    this->currToken_ = this->lexer_.nextToken();

    printf("\nPARSING...\n");
}

void Parser::consume(TokenType type) {
    // if (type != this->currToken_.type) printf("Consume: %s\n",
    //     TokenTypeStrings[static_cast<int>(type)]);
    assert(type == this->currToken_.type);
    this->currToken_ = this->lexer_.nextToken();
}

// parse : program
AST *Parser::parse() {
    AST *node = this->program();
    assert(this->currToken_.type == TokenType::END);
    
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
        case TokenType::INTEGER: // This seems like bad code
            return this->declaration();
        case TokenType::DOUBLE:
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
        case TokenType::INTEGER_CONST:
            this->consume(TokenType::INTEGER_CONST);
            return new Num(expr);
        case TokenType::DOUBLE_CONST:
            this->consume(TokenType::DOUBLE_CONST);
            return new Num(expr);
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
            assert(0);
    }
}

// assignment_expression = additive_expression
//                       | unary_expression assignment_operator
//                       assignment_expression
AST *Parser::assignment_expr() {
    Token lhs = this->currToken_;

    // This currently is a hacky fix
    // need to find a way for it to parse int x = a+2;
    if (lhs.type == TokenType::INTEGER_CONST ||
        lhs.type == TokenType::DOUBLE_CONST) {
        return additive_expr();
    }

    AST *left = this->unary_expr();

    Token assignmentOp = this->currToken_;
    this->consume(TokenType::ASSIGN);

    AST *right = this->additive_expr();
    return new Assign(*left, assignmentOp, *right);
}

// declaration = {type_specifier}+ identifier ("=" assignment_expression)? ";"
AST *Parser::declaration() {
    AST *type = this->type_spec();

    AST *id = new Var(this->currToken_);
    this->consume(TokenType::ID);

    if (this->currToken_.type == TokenType::ASSIGN) {
        // This should be removed for null declaration
        this->consume(TokenType::ASSIGN);

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
        default:
            assert(0);
    }

    return new Type(type);
}

// end parser

Interpreter::Interpreter(Parser parser) : parser_(std::move(parser)) {}

std::string Interpreter::toString() const {
    std::string out;

    for (const auto &x : this->GLOBAL) {
        out += x.first + ": " + std::to_string(x.second) + "\n";
    }

    return out;
}

int Interpreter::visit(const AST &node) {
    printf("visiting %s \n", ASTTypeStrings[static_cast<int>(node.type)]);
    switch (node.type) {
        case ASTType::Program:
            return visitProgram(static_cast<const Program &>(node));
        case ASTType::Compound:
            return visitCompound(static_cast<const Compound &>(node));
        case ASTType::BinOp:
            return visitBinOp(static_cast<const BinOp &>(node));
        case ASTType::UnaryOp:
            return visitUnaryOp(static_cast<const UnaryOp &>(node));
        case ASTType::NoOp:
            return 0;
        case ASTType::Assign:
            return visitAssign(static_cast<const Assign &>(node));
        case ASTType::VarDecl:
            return visitVarDecl(static_cast<const VarDecl &>(node));
        case ASTType::Type:
            return visitType(static_cast<const Type &>(node));
        case ASTType::Var:
            return visitVar(static_cast<const Var &>(node));
        case ASTType::Num:
            return visitNum(static_cast<const Num &>(node));
        default:
            assert(0);
    }
}

int Interpreter::visitProgram(const Program &node) {
    printf("%s\n", node.toString().c_str());

    for (AST &block : node.blocks) {
        this->visit(block);
    }
    return 0;
}

int Interpreter::visitCompound(const Compound &node) {
    printf("%s\n", node.toString().c_str());

    for (AST &child : node.children) {
        this->visit(child);
    }
    return 0;
}

int Interpreter::visitBinOp(const BinOp &node) {
    printf("%s\n", node.toString().c_str());

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
        case TokenType::INT_DIV:
            return (double)this->visit(node.left) / this->visit(node.right);
            break;
        case TokenType::REM:
            return this->visit(node.left) % this->visit(node.right);
            break;
        default:
            assert(0);
    }
}

int Interpreter::visitUnaryOp(const UnaryOp &node) {
    printf("%s\n", node.toString().c_str());

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

int Interpreter::visitAssign(const Assign &node) {
    printf("%s\n", node.toString().c_str());

    this->GLOBAL.emplace(node.left.token.value, this->visit(node.right));
    return 0;
}

int Interpreter::visitVarDecl(const VarDecl &node) {
    printf("%s\n", node.toString().c_str());

    if (node.expr.token.type == TokenType::END) {
        this->GLOBAL.emplace(node.id.token.value, -999);
    } else {
        this->GLOBAL.emplace(node.id.token.value, this->visit(node.expr));
    }

    return 0;
}

int Interpreter::visitType(const Type &node) {
    printf("%s\n", node.toString().c_str());

    return 0;
}

int Interpreter::visitVar(const Var &node) {
    printf("%s\n", node.toString().c_str());

    return this->GLOBAL.at(node.token.value); // throws
}

int Interpreter::visitNum(const Num &node) {
    printf("%s\n", node.toString().c_str());

    return std::stoi(node.token.value);
}

int Interpreter::interpret() {
    // search for new keyword
    AST *root = this->parser_.parse();

    printf("\nINTERPRETING...\n");
    printf("root node %s \n", root->token.toString().c_str());

    this->visit(*root);

    printf("symbol table \n%s \n", this->toString().c_str());

    return 0;
}
// end interpreter

} // namespace bluefin
