# hw3 report

|||
|-:|:-|
|Name|陳星宇|
|ID|109550060|

## How much time did you spend on this project

30 hours.

## Project overview

I used the visitor pattern to complete this project. `AstDumper.cpp` served as a visitor to traverse the AST, while other files served as nodes in the tree.
1. Modify the `yylval` (return value) from the scanner (`lex.l`). For example, integer should have `yylval.integer=strtol(yytext, NULL, 10)`, octal integer should have the same form but the base is 8. Real and scientific value should have `yylval.real=atof(yytext)`.
2. Define the type used (including all the nodes) in the `%code requires` section.
3. Declare the type that will be represented as a semantic value in the `%union` section.
**Note that vector should be saved as a pointer since it is a non-POD**
4. Specify each token with a type declared in the `%union` section inside the `%type` section.
5. Add `accept` and `visitChildNodes` to all the nodes so that they can inherit the abstract class `AstNode` (Except for Expression, as it won't be dumped by the Astdumper).
6. Follow the instructions on the `astguideline.md` and repeat the following part:
    - Return the semantic value of tokens, backtrack (and do upcasting when needed) and combine into a node class when it encounters the terminal token.
    - Modify the constructor of the node class and process something like constructor overloading or downcasting (if the arguments passed in is upcast).
    - Modify `Astdumper.cpp` and print the desired output.
    - Modify `visitChildNodes` to let the children accept the visiter.
    **The only exception is expression.cpp, it uses the accept function to downcast itself into its child and pass in the visitor**

## What is the hardest you think in this project

1. Visitor pattern. Although I've learned OOP and some casting techniques of inheritance, I've never learned any of the design pattern, so it takes a lot of time for me to figure out the flow of this assignment.
2. Memory issue, such as `char*`, `int*` and `float*`. As I used them to pass the `yylval` back to the parser and I forgot to malloc a new space, so some variable initialized later somehow points to the memory where those value are stored and leads to unexpected results (some of the identifiers will become invisible control characters). I've stucked for about 2 days.

## Feedback to T.A.s

The assignment is very informative, and it reminds me of the core of OOP (not simply just use the class as a structured type). And it is kind that TAs had provided some hints and guidelines to the assignment.

# POINTER SUCKS UWU
