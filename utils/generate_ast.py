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
ast_hpp = open("./include/asttest.hpp", "w")

ast_hpp.write("""
#pragma once

#include <functional>
#include <string>
#include <vector>
#include "token.hpp"

namespace bluefin {

""")

# write enums
ast_hpp.write("enum class ASTType : int {\n");
for k in ast_data:
    ast_hpp.write("    {},\n".format(k))
ast_hpp.write("};\n")

ast_hpp.write("static constexpr const char *const ASTTypeStrings[{}] = {{\n".format(len(ast_data)))
for k in ast_data:
    ast_hpp.write("    \"{}\",\n".format(k))
ast_hpp.write("};\n")

# write root class
ast_hpp.write("""
// Tree node parent class
class AST {
   public:
    AST();
    virtual void accept(Visitor *visitor) const = 0;
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

        void accept(Visitor *visitor) const;

    private:
    }};
    """.format(k, k, k, v, field_def))

# define visitor
ast_hpp.write("""
// Abstract visitor class 
class Visitor {
public:""")

for k in ast_data:
    ast_hpp.write("virtual void visit{}(const {} *node) const = 0;\n".format(k, k))
    
ast_hpp.write("\n};\n")

ast_hpp.write("\n} // namespace bluefin")

ast_hpp.close()

# DEFINE CPP FILE
ast_cpp = open("./src/asttest.cpp", "w")

ast_cpp.write("""
#include "asttest.hpp"

namespace bluefin {

""")

for k,v in ast_data.items():
    field_init = ", ".join(map(lambda x: "{}({})".format(x.replace(",", ""), x.replace(",", "")), re.findall("[a-zA-Z0-9_]+,|[a-zA-Z0-9_]+$", v)))
    if len(field_init) > 0:
        field_init = ":" + field_init

    ast_cpp.write("""
    {}::{}({}) {} {{}}

    void {}::accept(Visitor *visitor) const{{
        visitor->visit{}(this);
    }}

    """.format(k, k, v, field_init, k, k))

ast_cpp.write("\n} // namespace bluefin")

ast_cpp.close()

import subprocess
subprocess.call(["./utils/format.sh"])
