#include "bluefin.hpp"

namespace bluefin {
namespace {} // namespace

ASTTraverser::ASTTraverser() {}

ASTTraverser::ASTTraverser(AST *root) {
    this->root_ = root;
}

data_type_t ASTTraverser::visit(const AST &node) {
    printf("visiting %s \n", ASTTypeStrings[static_cast<int>(node.type)]);
    switch (node.type) {
        case ASTType::Program:
            visitProgram(static_cast<const Program &>(node));
        case ASTType::Compound:
            visitCompound(static_cast<const Compound &>(node));
        case ASTType::BinOp:
            visitBinOp(static_cast<const BinOp &>(node));
        case ASTType::UnaryOp:
            visitUnaryOp(static_cast<const UnaryOp &>(node));
        case ASTType::NoOp:
            break;
        case ASTType::Assign:
            visitAssign(static_cast<const Assign &>(node));
        case ASTType::VarDecl:
            visitVarDecl(static_cast<const VarDecl &>(node));
        case ASTType::Type:
            visitType(static_cast<const Type &>(node));
        case ASTType::Var:
            visitVar(static_cast<const Var &>(node));
        case ASTType::Num:
            visitNum(static_cast<const Num &>(node));
        default:
            throw new std::logic_error(
                "Interpreter found invalid AST node" + node.toString());
    }
    return;
}
// end ASTTraverser

Interpreter::Interpreter(AST *root, SymbolTable symbols) : symbols_(symbols), ASTTraverser(root) {}

std::string Interpreter::toString() const {
    std::string out = "Global vars: \n";

    for (const auto &x : this->global_) {
        out += x.first + ": ";
        for (int i = 0; i < x.second.size; i++) {
            char buffer[1];
            sprintf(buffer, "%02x", x.second.data[i]);
            out += std::string(buffer) + "\n";
        }
    }

    return out;
}

void Interpreter::visitProgram(const Program &node) {
    printf("%s\n", node.toString().c_str());

    for (const AST &block : node.blocks) {
        this->visit(block);
    }
}

void Interpreter::visitCompound(const Compound &node) {
    printf("%s\n", node.toString().c_str());

    for (AST &child : node.children) {
        this->visit(child);
    }
}

data_type_t Interpreter::visitBinOp(const BinOp &node) {
    printf("%s\n", node.toString().c_str());

    data_type_t res;
    res.size = 4;
    res.value.data = (char*)malloc(4);
    
    data_type_t left = this->visit(node.left);
    data_type_t right = this->visit(node.right);

    if (left.type.name == "double") {
        if (right.type.name == "int") {
            res.type = Symbol("double");
        } else {
            throw new std::logic_error("Interpreter found invalid binary op" + node.toString());
        }
    } else if (right.type.name == "double") {
        if (left.type.name == "int") {
            res.type = Symbol("double");
        } else {
            throw new std::logic_error("Interpreter found invalid binary op" + node.toString());
        }
    } else if (left.type.name == "int" && right.type.name == "int") {
        res.type == Symbol("int");
    } else {
        throw new std::logic_error("Interpreter found invalid binary op" + node.toString());
    }

    switch (node.token.type) {
        case TokenType::PLUS:
            res.value.data = left.getData() + right.getData();
            break;
        case TokenType::MINUS:
            res.value.data = left.getData() - right.getData();
            break;
        case TokenType::MULT:
            res.value.data = left.getData() * right.getData();
            break;
        case TokenType::DIV:
            res.value.data = (double)left.getData() / right.getData();
            break;
        case TokenType::INT_DIV:
            res.value.data = left.getData() / right.getData();
            break;
        case TokenType::REM:
            res.value.data = left.getData() % right.getData();
            break;
        default:
            throw new std::logic_error("Interpreter found invalid binary op" + node.toString());
    }

    return res;
}

data_type_t Interpreter::visitUnaryOp(const UnaryOp &node) {
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

void Interpreter::visitAssign(const Assign &node) {
    printf("%s\n", node.toString().c_str());

    this->global_.emplace(node.left.token.value, this->visit(node.right));
}

void Interpreter::visitVarDecl(const VarDecl &node) {
    printf("%s\n", node.toString().c_str());

    if (node.expr.token.type == TokenType::END) {
        this->global_.emplace(node.id.token.value, -999);
    } else {
        this->global_.emplace(node.id.token.value, this->visit(node.expr));
    }
}

void Interpreter::visitType(const Type &node) {
    printf("%s\n", node.toString().c_str());
}

data_type_t Interpreter::visitVar(const Var &node) {
    printf("%s\n", node.toString().c_str());

    return this->global_.at(node.token.value); // throws
}

data_type_t Interpreter::visitNum(const Num &node) {
    printf("%s\n", node.toString().c_str());

    return std::stoi(node.token.value);
}

void Interpreter::interpret() {
    printf("\nINTERPRETING...\n");

    printf("root node %s \n", this->root_->token.toString().c_str());

    this->visit(*this->root_);

    printf("\n%s \n", this->toString().c_str());
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

SymbolTable SymbolTableBuilder::getSymbols() const {
    return this->symbols_;
}

void SymbolTableBuilder::visitProgram(const Program &node) {
    for (const AST &block : node.blocks) {
        this->visit(block);
    }
}

void SymbolTableBuilder::visitCompound(const Compound &node) {
    for (AST &child : node.children) {
        this->visit(child);
    }
}

void SymbolTableBuilder::visitBinOp(const BinOp &node) {
    this->visit(node.left);
    this->visit(node.right);
}

void SymbolTableBuilder::visitUnaryOp(const UnaryOp &node) {
    this->visit(node.child);
}

void SymbolTableBuilder::visitAssign(const Assign &node) {
    std::string varName = node.left.token.value;
    this->symbols_.lookup(varName); // Throws if var hasn't been defined
    this->visit(node.right);
}

void SymbolTableBuilder::visitVarDecl(const VarDecl &node) {
    Symbol typeSymbol = this->symbols_.lookup(node.typeNode.token.value);
    VarSymbol varSymbol = VarSymbol(node.id.token.value, typeSymbol);
    this->symbols_.define(varSymbol);
}

void SymbolTableBuilder::visitType(const Type &node) {}

void SymbolTableBuilder::visitVar(const Var &node) {
    std::string varName = node.token.value;
    this->symbols_.lookup(varName); // Throws if var hasn't been defined
}

void SymbolTableBuilder::visitNum(const Num &node) {}

// end SymbolTableBuilder

} // namespace bluefin
