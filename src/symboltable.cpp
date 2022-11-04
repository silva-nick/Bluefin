#include "symboltable.hpp"

namespace bluefin {

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
    this->define(BuiltinTypeSymbol("string"));
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

SymbolTableBuilder::SymbolTableBuilder(AST *root, std::stringstream &buffer)
    : ASTTraverser(root), buffer_(buffer) {
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

int SymbolTableBuilder::visitString(const String &node) {
    return 0;
}
// end SymbolTableBuilder

} // namespace bluefin
