import re

ast_data = {
    "BinOp": "AST *left, Token *op, AST *right "
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

ast_hpp.write("static constexpr const char *const ASTTypeStrings[11] = {\n");
for k in ast_data:
    ast_hpp.write("    \"{}\",\n".format(k))
ast_hpp.write("};\n")

# write root class
ast_hpp.write("""
// Tree node parent class
class AST {
   public:
    AST();
    virtual void accept(Visitor &visitor) = 0;
   private:
};
""")

for k,v in ast_data.items():
    field_def = ";\n".join(v.split(", "))+";\n"
    ast_hpp.write("""
    // {} node
    class {} : public AST {{
    public:
        {}({});

        void accept(Visitor &visitor);

        {}
    private:
    }};
    """.format(k, k, k, v, field_def))

ast_hpp.write("\n} // namespace bluefin")

ast_hpp.close()

# DEFINE CPP FILE
ast_cpp = open("./src/asttest.cpp", "w")

ast_cpp.write("""
#include "asttest.hpp"

namespace bluefin {

""")

for k,v in ast_data.items():
    field_init = ", ".join(map(lambda x: "{}({})".format(x[:-1], x[:-1]), re.findall("[a-zA-Z0-9_]+,|[a-zA-Z0-9_]+$", v)))

    ast_cpp.write("""
    {}::{}({}): {} {{}}


    void {}::accept(Visitor &visitor) {{
        visitor.visit{}(this);
    }}

    """.format(k, k, v, field_init, k, str.capitalize(k)))

ast_cpp.write("\n} // namespace bluefin")

ast_cpp.close()

import subprocess
subprocess.call(["./utils/format.sh"])
