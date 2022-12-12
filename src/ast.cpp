#include "ast.hpp"

namespace bluefin {

AST::AST(ASTType type) : type(type) {}

BinOp::BinOp(ASTType type, AST *left, Token *op, AST *right)
    : AST(ASTType::BinOp), left(left), op(op), right(right) {}

boost::any BinOp::accept(Visitor *visitor) const {
    visitor->visitBinOp(this);
}

Double::Double(ASTType type, DoubleToken *value)
    : AST(ASTType::Double), value(value) {}

boost::any Double::accept(Visitor *visitor) const {
    visitor->visitDouble(this);
}

Program::Program(ASTType type, std::vector<AST *> compounds)
    : AST(ASTType::Program), compounds(compounds) {}

boost::any Program::accept(Visitor *visitor) const {
    visitor->visitProgram(this);
}

Var::Var(ASTType type, StringToken *varName)
    : AST(ASTType::Var), varName(varName) {}

boost::any Var::accept(Visitor *visitor) const {
    visitor->visitVar(this);
}

Type::Type(ASTType type, StringToken *typeName)
    : AST(ASTType::Type), typeName(typeName) {}

boost::any Type::accept(Visitor *visitor) const {
    visitor->visitType(this);
}

Assign::Assign(ASTType type, AST *left, Token *op, AST *right)
    : AST(ASTType::Assign), left(left), op(op), right(right) {}

boost::any Assign::accept(Visitor *visitor) const {
    visitor->visitAssign(this);
}

String::String(ASTType type, StringToken *value)
    : AST(ASTType::String), value(value) {}

boost::any String::accept(Visitor *visitor) const {
    visitor->visitString(this);
}

UnaryOp::UnaryOp(ASTType type, AST *node, Token *op)
    : AST(ASTType::UnaryOp), node(node), op(op) {}

boost::any UnaryOp::accept(Visitor *visitor) const {
    visitor->visitUnaryOp(this);
}

VarDecl::VarDecl(ASTType type, AST *typeNode, AST *id, AST *expr)
    : AST(ASTType::VarDecl), typeNode(typeNode), id(id), expr(expr) {}

boost::any VarDecl::accept(Visitor *visitor) const {
    visitor->visitVarDecl(this);
}

NoOp::NoOp(ASTType type) : AST(ASTType::NoOp) {}

boost::any NoOp::accept(Visitor *visitor) const {
    visitor->visitNoOp(this);
}

Compound::Compound(ASTType type, std::vector<AST *> statements)
    : AST(ASTType::Compound), statements(statements) {}

boost::any Compound::accept(Visitor *visitor) const {
    visitor->visitCompound(this);
}

Integer::Integer(ASTType type, IntegerToken *value)
    : AST(ASTType::Integer), value(value) {}

boost::any Integer::accept(Visitor *visitor) const {
    visitor->visitInteger(this);
}

} // namespace bluefin