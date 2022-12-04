#include "ast.hpp"

namespace bluefin {

BinOp::BinOp(AST *left, Token *op, AST *right)
    : left(left), op(op), right(right) {}

boost::any BinOp::accept(Visitor *visitor) const {
    visitor->visitBinOp(this);
}

Double::Double(DoubleToken *value) : value(value) {}

boost::any Double::accept(Visitor *visitor) const {
    visitor->visitDouble(this);
}

Program::Program(std::vector<AST *> compounds) : compounds(compounds) {}

boost::any Program::accept(Visitor *visitor) const {
    visitor->visitProgram(this);
}

Var::Var(StringToken *varName) : varName(varName) {}

boost::any Var::accept(Visitor *visitor) const {
    visitor->visitVar(this);
}

Type::Type(StringToken *typeName) : typeName(typeName) {}

boost::any Type::accept(Visitor *visitor) const {
    visitor->visitType(this);
}

Assign::Assign(AST *left, Token *op, AST *right)
    : left(left), op(op), right(right) {}

boost::any Assign::accept(Visitor *visitor) const {
    visitor->visitAssign(this);
}

String::String(StringToken *value) : value(value) {}

boost::any String::accept(Visitor *visitor) const {
    visitor->visitString(this);
}

UnaryOp::UnaryOp(AST *node, Token *op) : node(node), op(op) {}

boost::any UnaryOp::accept(Visitor *visitor) const {
    visitor->visitUnaryOp(this);
}

VarDecl::VarDecl(AST *typeNode, AST *id, AST *expr)
    : typeNode(typeNode), id(id), expr(expr) {}

boost::any VarDecl::accept(Visitor *visitor) const {
    visitor->visitVarDecl(this);
}

NoOp::NoOp() {}

boost::any NoOp::accept(Visitor *visitor) const {
    visitor->visitNoOp(this);
}

Compound::Compound(std::vector<AST *> statements) : statements(statements) {}

boost::any Compound::accept(Visitor *visitor) const {
    visitor->visitCompound(this);
}

Integer::Integer(IntegerToken *value) : value(value) {}

boost::any Integer::accept(Visitor *visitor) const {
    visitor->visitInteger(this);
}

} // namespace bluefin