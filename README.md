## Bluefin :fish:

> A C-style block-based interpreted computer language

#### Building with Docker

**`Run docker container`**

```bash
docker compose up -d --build
docker cp bf:/usr/include/boost/ ./extern/boost
docker exec -ti bf bash
```

**use this:**

```bash

./utils/config.sh

./utils/build.sh
```


**Probably just use build.sh in ./utils**

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

#### Grammar (EBNF)

```
program = compound_statement
        | program compound_statement

compound_statement = "{" statement_list "}"

statement_list = statement
               | statement statement_list

statement = compound_statement line_end
          | assignment_expression line_end
          | white_space

expression = assignment_expression

assignment_expression = logical_expression
                      | unary_expression assignment_operator assignment_expression

logical_expression = relational_expression
                   | logical_expression || relational_expression
                   | logical_expression && relational_expression

relational_expression = additive_expression
                      | relational_expression == additive_expression
                      | relational_expression != additive_expression
                      | relational_expression >= additive_expression
                      | relational_expression <= additive_expression
                      | relational_expression > additive_expression
                      | relational_expression < additive_expression

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

identifier = alphabetic character (alphanumeric character)*

declaration = {type_specifier}+ {mode_specifier}+ identifier assignment_operator assignment_expression line_end

type_specifier = int
               | double
               | string

mode_specifier = ?
               | ~

constant = integer      
         | double
         | "string"

assignment_operator = =

unary_operator = +|-

line_end = ";"|"\n"
```

#### Credit:

- https://ruslanspivak.com/lsbasi-part1/
- https://github.com/awh/compsci-papers/blob/master/Fundamental%20Concepts%20in%20Programming%20Languages.pdf
- https://cs.wmich.edu/~gupta/teaching/cs4850/sumII06/The%20syntax%20of%20C%20in%20Backus-Naur%20form.htm
