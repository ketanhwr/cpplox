#!/usr/bin/env python3

import sys

def define_ast(output_dir, base_class, ast_types, dependencies = []):
    with open(file=f"{output_dir}/{str.lower(base_class)}.hpp", mode="w") as output_file:
        output_file.write("#pragma once\n\n")

        for dependecy in dependencies:
            output_file.write(f"#include \"{dependecy}\"\n\n")

        for k in ast_types:
            output_file.write(f"struct {k}{base_class};\n");
        output_file.write("\n");

        output_file.write(f"struct {base_class}\n")
        output_file.write("{\n")

        # AbstractVisitor structure
        output_file.write("\tstruct AbstractVisitor\n")
        output_file.write("\t{\n")

        for k in ast_types:
            output_file.write(f"\t\tvirtual void visit{k}{base_class}({k}{base_class}& {str.lower(base_class)}) = 0;\n")

        output_file.write("\t};\n\n")

        output_file.write("\tvirtual void accept(AbstractVisitor& visitor) = 0;\n")
        output_file.write(f"\tvirtual ~{base_class}() = default;\n")
        output_file.write("};\n\n")

        for k, v in ast_types.items():
            attrs = [[x.strip().split()[0].strip(), x.strip().split()[1].strip()] for x in v.split('|') ]

            output_file.write(f"struct {k}{base_class}: public {base_class}\n")
            output_file.write("{\n")

            for var in attrs:
                output_file.write(f"\tstd::shared_ptr<{var[0]}> {var[1]}_;\n")

            output_file.write("\n")

            # Constructor
            c_args = ", ".join([f"std::shared_ptr<{x[0]}> {x[1]}" for x in attrs])
            init_args = "\t\t, ".join([f"{x[1]}_{{std::move({x[1]})}}\n" for x in attrs])

            output_file.write(f"\t{k}{base_class}({c_args})\n")
            output_file.write(f"\t\t: {init_args}")
            output_file.write("\t{ }\n\n")

            # Override accept() function
            output_file.write("\tvoid accept(AbstractVisitor& visitor) override {\n")
            output_file.write(f"\t\tvisitor.visit{k}{base_class}(*this);\n")
            output_file.write("\t}\n")

            output_file.write("};\n\n")

def main():
    if len(sys.argv) != 2:
        print (f"Usage: {sys.argv[0]} <out-dir>")
        sys.exit(1)

    define_ast(sys.argv[1], "Expr", {
        "Assign": "Token name | Expr value",
        "Binary": "Expr left | Token op | Expr right",
        "Grouping": "Expr expression",
        "Literal": "LoxValue value",
        "Unary": "Token op | Expr right",
        "Variable": "Token name"
    }, [ "token.hpp" ])

    define_ast(sys.argv[1], "Stmt", {
        "Expression": "Expr expression",
        "Print": "Expr expression",
        "Var": "Token name | Expr initializer"
    }, [ "expr.hpp" ])

if __name__ == "__main__":
    main()

