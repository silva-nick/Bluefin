## Bluefin :fish:

> A C-style block-based interpreted computer language

#### Grammar (EBNF)

```
program = compound_statement
        | program compound_statement

compound_statement = "{" statement_list "}"

statement_list = statement
               | statement statement_list

statement = compound_statement ";" 
          | assignment_expression ";" 
          | white_space

additive_expression = multiplicative_expression 
                    | additive_expression + multiplicative_expression
                    | additive_expression - multiplicative_expression

multiplicative_expression = unary_expression
                          | multiplicative_expression * unary_expression
                          | multiplicative_expression / unary_expression
                          | multiplicative_expression // unary_expression
                          | multiplicative_expression % unary_expression

unary_expression = primary_expression
                 | unary_operator primary_expression

primary_expression = indentifier
                   | constant
                   | "(" expression ")"

expression = assignment_expression

assignment_expression = additive_expression
                      | unary_expression assignment_operator assignment_expression

identifier = alphabetic character (alphanumeric character)*

declaration = {type_specifier}+ identifier "=" assignment_expression ";"

type_specifier = int
               | double

constant = integer      
         | double

assignment_operator = =

unary_operator = +|-
```

#### Building with Docker

**`Run docker container`**

```bash
docker compose up -d --build
docker exec -ti bf bash
```

_Then to build._
```bash
# Configure the build
CC=gcc CXX=g++ cmake ..
cmake --build build --target bluefin
```

_Testing_
```bash
ctest -C Release
```

**`Debug`**
```bash
cmake -S . -B build/Debug -D CMAKE_BUILD_TYPE=Debug
cmake --build build/Debug --target test
```

#### Credit:

- https://ruslanspivak.com/lsbasi-part1/
- https://github.com/awh/compsci-papers/blob/master/Fundamental%20Concepts%20in%20Programming%20Languages.pdf
- https://cs.wmich.edu/~gupta/teaching/cs4850/sumII06/The%20syntax%20of%20C%20in%20Backus-Naur%20form.htm
