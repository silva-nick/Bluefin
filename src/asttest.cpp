
#include "asttest.hpp"

namespace bluefin {

BinOp::BinOp(AST *left, Token *op, AST *right)
    : left(left), op(op), right(right) {}

void BinOp::accept(Visitor *visitor) const {
    visitor->visitBinOp(this);
}

Double::Double(DoubleToken *value) : value(value) {}

void Double::accept(Visitor *visitor) const {
    visitor->visitDouble(this);
}

Program::Program(std::vector<AST *> compounds) : compounds(compounds) {}

void Program::accept(Visitor *visitor) const {
    visitor->visitProgram(this);
}

Var::Var(StringToken *varName) : varName(varName) {}

void Var::accept(Visitor *visitor) const {
    visitor->visitVar(this);
}

Type::Type(StringToken *typeName) : typeName(typeName) {}

void Type::accept(Visitor *visitor) const {
    visitor->visitType(this);
}

Assign::Assign(AST *left, Token *op, AST *right)
    : left(left), op(op), right(right) {}

void Assign::accept(Visitor *visitor) const {
    visitor->visitAssign(this);
}

String::String(StringToken *value) : value(value) {}

void String::accept(Visitor *visitor) const {
    visitor->visitString(this);
}

UnaryOp::UnaryOp(AST *node, Token *op) : node(node), op(op) {}

void UnaryOp::accept(Visitor *visitor) const {
    visitor->visitUnaryOp(this);
}

VarDecl::VarDecl(AST *typeNode, AST *id, AST *expr)
    : typeNode(typeNode), id(id), expr(expr) {}

void VarDecl::accept(Visitor *visitor) const {
    visitor->visitVarDecl(this);
}

NoOp::NoOp() {}

void NoOp::accept(Visitor *visitor) const {
    visitor->visitNoOp(this);
}

Compound::Compound(std::vector<AST *> statements) : statements(statements) {}

void Compound::accept(Visitor *visitor) const {
    visitor->visitCompound(this);
}

Integer::Integer(IntegerToken *value) : value(value) {}

void Integer::accept(Visitor *visitor) const {
    visitor->visitInteger(this);
}

} // namespace bluefin