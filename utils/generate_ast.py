import re

ast_data = {
    "Program": "std::vector<AST*> compounds",
    "Compound": "std::vector<AST*> statements",
    "Assign": "AST *left, Token *op, AST *right",
    "BinOp": "AST *left, Token *op, AST *right",
    "UnaryOp": "AST *node, Token *op",
    "NoOp": "",
    "VarDecl": "AST *typeNode, AST *id, AST *expr",
    "Type": "StringToken *typeName",
    "Var": "StringToken *varName",
    "Integer": "IntegerToken *value",
    "Double": "DoubleToken *value",
    "String": "StringToken *value",
}

# DEFINE HEADER FILE
ast_hpp = open("./include/ast.hpp", "w")

ast_hpp.write(
"""#pragma once

#include <functional>
#include <string>
#include <vector>
#include <boost/any.hpp>
#include "token.hpp"

namespace bluefin {

class Visitor;

// Tree node parent class
class AST {
   public:
    virtual ~AST() {}
    virtual boost::any accept(Visitor *visitor) const = 0;
   private:
};
""")

for k,v in ast_data.items():
    field_def = ";\n".join(v.split(", "))
    if field_def is not "":
        field_def += ";\n"

    ast_hpp.write("""
    // {} node
    class {} : public AST {{
    public:
        {}({});
        {}

        boost::any accept(Visitor *visitor) const;

    private:
    }};
    """.format(k, k, k, v, field_def))

# define visitor
ast_hpp.write("""
// Abstract visitor class 
class Visitor {
public:""")

for k in ast_data:
    ast_hpp.write("virtual boost::any visit{}(const {} *node) = 0;\n".format(k, k))
    
ast_hpp.write("\n};")

ast_hpp.write("""

StringToken *asStringToken(const AST *node);

""")

ast_hpp.write("\n} // namespace bluefin")

ast_hpp.close()

# DEFINE CPP FILE
ast_cpp = open("./src/ast.cpp", "w")

ast_cpp.write(
"""#include "ast.hpp"

namespace bluefin {

""")

for k,v in ast_data.items():
    field_init = ", ".join(map(lambda x: "{}({})".format(x.replace(",", ""), x.replace(",", "")), re.findall("[a-zA-Z0-9_]+,|[a-zA-Z0-9_]+$", v)))
    if len(field_init) > 0:
        field_init = ":" + field_init

    ast_cpp.write("""
    {}::{}({}) {} {{}}

    boost::any {}::accept(Visitor *visitor) const{{
        return visitor->visit{}(this);
    }}

    """.format(k, k, v, field_init, k, k))

ast_cpp.write("""

StringToken *asStringToken(const AST *node) {
    return (StringToken *)((String *)node)->value;
}

""")

ast_cpp.write("\n} // namespace bluefin")

ast_cpp.close()

import subprocess
subprocess.call(["./utils/format.sh"])
