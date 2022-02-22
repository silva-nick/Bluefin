#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

ASTTraverser::ASTTraverser() {}

ASTTraverser::ASTTraverser(AST *root) {
    this->root_ = root;
}

int ASTTraverser::visit(const AST &node) {
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
            throw new std::logic_error(
                "Interpreter found invalid AST node" + node.toString());
    }
}
// end ASTTraverser

Interpreter::Interpreter(AST *root) : ASTTraverser(root) {}

std::string Interpreter::toString() const {
    std::string out = "Global vars: \n";

    for (const auto &x : this->global_) {
        out += x.first + ": " + std::to_string(x.second) + "\n";
    }

    return out;
}

int Interpreter::visitProgram(const Program &node) {
    printf("%s\n", node.toString().c_str());

    for (const AST &block : node.blocks) {
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
            throw new std::logic_error(
                "Interpreter found invalid binary op" + node.toString());
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
            throw new std::logic_error(
                "Interpreter found invalid unary op" + node.toString());
    }
}

int Interpreter::visitAssign(const Assign &node) {
    printf("%s\n", node.toString().c_str());

    this->global_.emplace(node.left.token.value, this->visit(node.right));
    return 0;
}

int Interpreter::visitVarDecl(const VarDecl &node) {
    printf("%s\n", node.toString().c_str());

    if (node.expr.token.type == TokenType::END) {
        this->global_.emplace(node.id.token.value, -999);
    } else {
        this->global_.emplace(node.id.token.value, this->visit(node.expr));
    }

    return 0;
}

int Interpreter::visitType(const Type &node) {
    printf("%s\n", node.toString().c_str());

    return 0;
}

int Interpreter::visitVar(const Var &node) {
    printf("%s\n", node.toString().c_str());

    return this->global_.at(node.token.value); // throws
}

int Interpreter::visitNum(const Num &node) {
    printf("%s\n", node.toString().c_str());

    return std::stoi(node.token.value);
}

int Interpreter::interpret() {
    printf("\nINTERPRETING...\n");

    printf("root node %s \n", this->root_->token.toString().c_str());

    this->visit(*this->root_);

    printf("\n%s \n", this->toString().c_str());

    return 0;
}
// end interpreter

Symbol::Symbol(std::string name) : name(name) {}

std::string Symbol::toString() const {
    return "Symbol<" + name + ">";
}

BuiltinTypeSymbol::BuiltinTypeSymbol(std::string name) : Symbol(name) {}

VarSymbol::VarSymbol(std::string name, Symbol type)
    : Symbol(name), type(type) {}

std::string VarSymbol::toString() const {
    return "Symbol<" + name + ", " + type.toString() + ">";
}
// end Symbol

SymbolTable::SymbolTable() {
    this->initBuiltins();
}

void SymbolTable::initBuiltins() {
    this->define(BuiltinTypeSymbol("int"));
    this->define(BuiltinTypeSymbol("double"));
}

std::string SymbolTable::toString() const {
    std::string out = "Symbol Table:\n";

    for (const auto &x : this->symbols_) {
        out += x.first + ": " + x.second.toString() + "\n";
    }

    return out;
}

void SymbolTable::define(const Symbol &symbol) {
    printf("Define: %s\n", symbol.toString().c_str());
    this->symbols_[symbol.name] = std::move(symbol);
}
Symbol SymbolTable::lookup(const std::string &name) {
    printf("Lookup: %s\n", name.c_str());
    return this->symbols_.at(name);
}
// end SymbolTable

SymbolTableBuilder::SymbolTableBuilder(AST *root) : ASTTraverser(root) {
    printf("\nBUILDING SYMBOLS\n");
    this->build();
    printf("\n%s \n", this->toString().c_str());
}

void SymbolTableBuilder::build() {
    this->visit(*this->root_);
    return;
}

std::string SymbolTableBuilder::toString() const {
    return this->symbols_.toString();
}

int SymbolTableBuilder::visitProgram(const Program &node) {
    for (const AST &block : node.blocks) {
        this->visit(block);
    }
    return 0;
}

int SymbolTableBuilder::visitCompound(const Compound &node) {
    for (AST &child : node.children) {
        this->visit(child);
    }
    return 0;
}

int SymbolTableBuilder::visitBinOp(const BinOp &node) {
    this->visit(node.left);
    this->visit(node.right);
    return 0;
}

int SymbolTableBuilder::visitUnaryOp(const UnaryOp &node) {
    this->visit(node.child);
    return 0;
}

int SymbolTableBuilder::visitAssign(const Assign &node) {
    std::string varName = node.left.token.value;
    this->symbols_.lookup(varName); // Throws if var hasn't been defined
    this->visit(node.right);
    return 0;
}

int SymbolTableBuilder::visitVarDecl(const VarDecl &node) {
    Symbol typeSymbol = this->symbols_.lookup(node.typeNode.token.value);
    VarSymbol varSymbol = VarSymbol(node.id.token.value, typeSymbol);
    this->symbols_.define(varSymbol);
    return 0;
}

int SymbolTableBuilder::visitType(const Type &node) {
    return 0;
}

int SymbolTableBuilder::visitVar(const Var &node) {
    std::string varName = node.token.value;
    this->symbols_.lookup(varName); // Throws if var hasn't been defined
    return 0;
}

int SymbolTableBuilder::visitNum(const Num &node) {
    return 0;
}

// end SymbolTableBuilder

} // namespace bluefin
