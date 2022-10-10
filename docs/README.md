# Bluefin

> A statically typed introspective programming language.

:fish: :fish: :fish:

### New ideas:

-   Everything is mockable
    -   myClass.when().function().then(true)
-   For loop works like a wrapper class
    -   Scoped monkey patching
    -   For (I in x):
        -   I.index
    -   Same with map iterators
    -   Extension methods?
-   Data class
-   Mutable keyword
-   Saturated / desaturated classes
    -   Provide subset of required params
    -   Only allows functionality of these areas
    -   Option to saturate on call (expensive)
-   Object predicates: make assertions about an objects state
    -   Assert built in (promises)
    -   Num.assert(1-10)

### Introduction:

Each year record numbers of students are graduating from colleges with computer science degrees. As such, swarms of new developers are searching for new technologies to learn and adapt. This overwhelming pressure has moved the field of computer science to a critical juncture between legacy technology its contemporary rivals.

This trend in people hoping to learn programming is also widely paralleled by the growing scale and presence of code in every facet of modern life. TVs, Fridges, and other IoT devices, as well as brick- and-mortar companies fighting to establish a presence on the internet, have all pushed the software industry to move and evolve rapidly (Zia 2020). Moving so quickly, new ideas flow through the industry constantly changing what professional developers want out of programming languages.

The importance of these demands is captured quite adequately by the emergence of Python as the dominant programming language in computational fields as well as in higher institutions (Woodie 2021). Since the development of the popular languages of the ‘90s (C, C++, Fortran, etc.), available memory and computing power have increased by orders of magnitude. This trend has allowed very high-level languages, like Python, to take advantage of resources and provide a more accessible developer experience.

### Bluefin:

The purpose of this report is to outline the specification and design strategy of Bluefin, a statically- typed and introspective scripting language. Bluefin looks to take advantage of the flexibility provided by scripting languages while also building in more stringent design patterns into the language. This way, Bluefin pulls from procedural, object-oriented, and reflective programming paradigms to give developers flexibility and stability on large projects. Bluefin’s target audience ranges from rookie programmers learning their first or second language to seasoned developers frustrated with the lack of malleability in their chosen language.

The design of Bluefin is differentiated by three critical features: static typing, introspection, and scripting. Dynamic typing allows a programmer to avoid specifying the type of a variable upon declaration. This means that code that allows dynamic typing gives the programmer flexibility in how they pass and arrange their variables. It is a powerful abstraction that allows any variable name to take on any value regardless of how the memory has to be managed behind the scenes. This differs from static typing. With static typing, the method used by more traditional languages, the programmer must specify what kind of data will be stored inside each variable as the variable is being declared. This allows the compiler to make compile-time decisions about the variables that might not be possible were the items not statically typed. Static typing leads to a very explicit programming style where the developer must be very pedantic about their use of variables and makes the code inherently easier to read. Statically typed languages also force programmers to understand what kind of data type they are utilizing for their tasks. Finally, static typing can catch hard-to-miss runtime errors inevitable in large-scale programs. Bluefin, then, implements a strict static typing system.

Dynamic typing, however, leads to one interesting design principle: introspection. As types cannot be explicitly defined in other places, sometimes this leads to confusion regarding an object’s type or API. To get around this common problem, many dynamically typed languages allow the programmer to access, at runtime, information about the objects there are handling. For example, a function passed a random object could first check to see if that object fulfills a specific primitive before handling the object. This allows for powerful abstractions and the ability to model compile-time features in an interpreted language. Moreover, by allowing object primitives to be optional, implementations can choose to use only a subset of the optional primitives. Then, an object consumer must employ introspection on the object to adapt its implementation through the provided interface.

The final powerful feature available in Bluefin is scripting. As the modern coding workflow moves towards segmentable notebooks and other script-based forms, developers have begun to demand a more iterative programming experience. Through Bluefin blocks, users can run code and get useful diagnostics before shipping to larger-scale ideas.

### Design Goals:

The section above provides some justification for why Bluefin has its fundamental features. In continuation of this, Bluefin is developed with a simple set of development goals that outline the intentions of the project into the future. These main ideas are summarized below.

#### Design goals of Bluefin

1. Enable and mandate type-safe programming practices.
2. Make writing code fast and simple to understand.
3. Allow the code to be self-documenting through readable syntax and strict typing.
4. Encourage memory-safe code by abstracting memory allocation for the user.
5. Allow an easy transition from C, C++, or other similar languages by adopting C-like syntax.
6. Provide a segmentable workflow that prioritizes iterative practices
7. Support easy and intuitive object-oriented programming primitives.
8. Use a context-free grammar that is intuitive and performant.
9. Do not sacrifice performance.
10. Provide powerful abstraction through introspection.
11. Ensure Bluefin is open source and accessible.

Each of these goals was crafted to support the fundamental goal of Bluefin, to provide a performant, educational, and syntactically powerful language. The importance of performance to the language is represented in points (8,9,11). In not sacrificing on performance, Bluefin scales to large projects and complex computational tasks. However, usability is incredibly important to Bluefin’s position as an educational computer language. Ease of access and programming, summarized in points (1,2,3,4,5,6,7,8), is guaranteed through maintaining recognizable syntax while simplifying cumbersome aspects of traditional languages. Moreover, by a computer languages Occam’s Razor doing things the best way is always easier than doing them the wrong way. Abstractly, Bluefin can be used in unique ways to other similar languages through iterative code blocks and introspection (10). This transition in development workflow gives users more power over how they want to develop. Finally, Bluefin is organized to give the developer extreme power and control over their code. Through layered abstraction and runtime introspection, Bluefin looks to empower developers in their environments.

# Details:

## Build-in types:

## Declarations:

## Inheritance:

## Scopes:

## Expressions:

## Statements:

## Variables:

## Control Flow:

## Functions:

## Classes:

### Methods:

Bluefin is an interpreted language and therefore needs an interpreter to run code and produce output. The Bluefin codebase uses C++17. The Bluefin interpreter will be made out of three fundamental units: the lexer, parser, and interpreter. The lexer, or tokenizer, is the object that reads in the given code as text and transforms it into tokens. It uses the language grammar to tokenize atomic character sequences from the input code. This functionality is utilized by the second component, the parser. The parser takes the stream of tokens and forms them into an abstract syntax tree. In this tree, programs, statements and expressions are implemented as DFS traversals of the tree’s nodes. Traversing this tree and creating the desired output is the goal of the interpreter. The interpreter takes as input the root node of the AST and traverses it. During this traversal, the interpreter runs the program. Each step of this interpreter design requires careful implementation and many unit tests.

### Diagram:

To implement the more advanced features of Bluefin, more research and experimentation are required.

# Bluefin EBNF Grammar (as of 2/2/22):

The language’s grammar, referenced many times above, is defined below in Extended Bakus-Naur Form (EBNF). EBNF is a notation used to define context-free grammars. The Bluefin grammar is roughly based off Ruslan Spivak’s Pascal interpreter and the C official language syntax. The grammar below is used to help guide the development process and can be used to completely implement a Bluefin parser.
program = compound*statement
| program compound_statement
compound* statement = "{" statement_list "}"
statement_list = statement
| statement statement_list
statement = compound_statement ";"
| assignment_expression ";"
| white_space
additive_expression = multiplicative_expression
| additive_expression + multiplicative_expression
| additive_expression - multiplicative_expression

multiplicative*expression = unary_expression
| multiplicative_expression * unary*expression
| multiplicative_expression / unary_expression
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
constant = integer
assignment_operator = =
unary_operator = +|-

# Resources:

Alexander, A. (2009). Java design goals. Introduction to Java and OOA/OOD for Web Applications. https://alvinalexander.com/java/misc/4days_dd/node47.shtml

Gupta, A. (2006). The syntax of C in Backus-Naur Form. Western Michigan. https://cs.wmich.edu/~gupta/teaching/cs4850/sumII06/The%20syntax%20of%20C%20in%20Backus-Naur%20form.htm

Marat Akhin, Mikhail Belyaev et al. “Kotlin language specification: Kotlin/Core”, JetBrains / JetBrains Research, 2020
Overview - D Programming Language. (2022). DLang. https://dlang.org/overview.html

Python Institute. (2017). About Python. https://pythoninstitute.org/what-is-python/

Qureshi, Z. (2020, February 27). Technology and the future of growth: Challenges of change. Brookings. https://www.brookings.edu/blog/up-front/2020/02/25/technology-and-the-future-of-growth-challenges-of-change/

Spivak, R. (2015, June 15). Let’s Build A Simple Interpreter. Part 1. Ruslan’s Blog. https://ruslanspivak.com/lsbasi-part1/

Strachey, C. (1967). Fundamental Concepts in Programming Languages. Kluwer Academic Publishers.

Woodie, A. (2021, October 21). What’s Driving Python’s Massive Popularity? Datanami. https://www.datanami.com/2021/10/20/whats-driving-pythons-massive-popularity/
