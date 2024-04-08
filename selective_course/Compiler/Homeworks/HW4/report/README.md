# hw4 report

|||
|-:|:-|
|Name|陳星宇|
|ID|109550060|

## How much time did you spend on this project

> e.g. 30 hours.

## Project overview

- Same as the last assignment, we use semantic analyzer (which serves as a visitor) to visit the AST. However, we focused on how to pass the semantic value and type from child to parent this time. For example, we have to add some protected value in expression so that all the children classes can store their type, dimension inside the class and will be taken out from its parent later.
- We also need to maintain a symbol table with several scopes, which contains not only the information of variables, but also its level and return type (to do semantic checking in `ReturnNode`).
- Also, we need to define some global value (using `extern`), such as symbol table listing option (declared in scanner and declared extern in `SemanticAnalyzer`) and source code line listing when there's semantic error (also declared in scanner and declared extern in `SemanticAnalyzer`)

## What is the hardest you think in this project

Debugging those bugs is the most annoying part, especially when I realize I actually don't understand some OOP structure.
- For example: I didn't realize that `for(auto &expr: expressions)` will use the interface of `ExpressionNode` but implicitly converted to `XXXNode` (child of `ExpressionNode`). I had to `static_cast/dynamic_cast` it into its real type so that I can access some property specific to the class.
- The other is that, due to unfamiliarity with visitor pattern, I write all the value passing inside the `XXXNode` in the beginning, and then I notice that the semantic parsing should not be done there as ASTDumper will also pass it (By then I've already done testcase 6 QwQ).

## Feedback to T.A.s

Thanks for the clarification of the spec on GitHub! HW3 and HW4 are really hard to do and takes me a lot of time, but I feel I'm getting more and more familiar to ~~OOP~~ compiler.