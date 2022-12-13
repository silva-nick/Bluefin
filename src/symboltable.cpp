#include "symboltable.hpp"
#include "bluefin.hpp"

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
    this->define(BuiltinTypeSymbol("error"));
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

Symbol SymbolTable::lookup(const StringToken *name, std::stringstream &buffer)
    const {
    printf("Lookup: %s\n", name->value.c_str());

    if (this->symbols_.count(name->value) == 0) {
        error(name->line, "Variable doesn't exist: " + name->value, buffer);
        printf("Im not a dumbt bitch i all about this shit fuck that shit \n\n\n\n\n");
        return this->symbols_.at("error");
    } else {
        return this->symbols_.at(name->value);
    }
}
// end SymbolTable

SymbolTableBuilder::SymbolTableBuilder(std::stringstream &buffer)
    : buffer_(buffer) {}

void SymbolTableBuilder::build(const AST *root) {
    printf("\nBUILDING SYMBOLS\n");
    root->accept((Visitor *)this);
    printf("\n%s \n", this->toString().c_str());
    return;
}

std::string SymbolTableBuilder::toString() const {
    return this->symbols_.toString();
}

boost::any SymbolTableBuilder::visitProgram(const Program *node) {
    for (const AST *compound : node->compounds) {
        compound->accept((Visitor *)this);
    }
    return boost::any();
}

boost::any SymbolTableBuilder::visitCompound(const Compound *node) {
    for (AST *statement : node->statements) {
        statement->accept((Visitor *)this);
    }
    return boost::any();
}

boost::any SymbolTableBuilder::visitBinOp(const BinOp *node) {
    node->left->accept((Visitor *)this);
    node->right->accept((Visitor *)this);

    return boost::any();
}

boost::any SymbolTableBuilder::visitUnaryOp(const UnaryOp *node) {
    node->node->accept((Visitor *)this);
    return boost::any();
}

boost::any SymbolTableBuilder::visitAssign(const Assign *node) {
    // Throws if var hasn't been defined
    symbols_.lookup(asStringToken(node->left), this->buffer_);

    // TODO: check if assigning to correct type
    node->right->accept((Visitor *)this);
    return boost::any();
}

boost::any SymbolTableBuilder::visitVarDecl(const VarDecl *node) {
    Symbol typeSymbol =
        this->symbols_.lookup(asStringToken(node->typeNode), this->buffer_);

    VarSymbol varSymbol = VarSymbol(asStringToken(node->id)->value, typeSymbol);

    node->expr->accept((Visitor *)this);

    // TODO: check if assigning to correct type
    this->symbols_.define(varSymbol);
    return boost::any();
}

boost::any SymbolTableBuilder::visitType(const Type *node) {
    return boost::any();
}

boost::any SymbolTableBuilder::visitVar(const Var *node) {
    this->symbols_.lookup((StringToken *)node->varName, this->buffer_);
    return boost::any();
}

boost::any SymbolTableBuilder::visitNoOp(const NoOp *node) {
    return boost::any();
}

boost::any SymbolTableBuilder::visitInteger(const Integer *node) {
    return boost::any();
}

boost::any SymbolTableBuilder::visitDouble(const Double *node) {
    return boost::any();
}

boost::any SymbolTableBuilder::visitString(const String *node) {
    return boost::any();
}
// end SymbolTableBuilder

} // namespace bluefin
