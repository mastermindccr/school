# hw2 report

|||
|-:|:-|
|Name|陳星宇|
|ID|109550060|

## How much time did you spend on this project

8 hours.

## Project overview

- scanner.l
    - First group tokens with similar traits together, such as '<', '>', '<>', '=', '<=', '>=', which can be classified as COMP. Return COMP to parser when tokens above are scanned. Other groups consists of INT(decimal and octal), REAL(float and scientific) etc.
    - Include "parser.h" to link TOKEN to the parser table.

- parser.y
    - Use %token to declare all the tokens used as terminal and use %start to declare the start symbol.
    - Use %left, %right to declare association rule of each operator.
    - Write rule using <non-terminal>: <several terminals or non-terminals>
    - When encountering rule with "zero or more", decompose it into empty or new rule that contains no empty(one or more).
    - When encountering rule with "one or more", decompose it using "S_LIST: S <delimiter> S_LIST | S".(Almost all the CFGs can apply this rule and the previous rules to complete)
    - One Exception is that the arithmetic expression has to deal with unary operator '-', which contradicts with the symbol used as subtraction. Unary operator has precedence over any other operators; thus, we need to use %prec to declare its priority.
     

## What is the hardest you think in this project

Sometimes I will miss some details, such as number literals contain negative numbers, so token "INT" and "REAL" should do some extra work to further decompose into "INT: +INT | -INT | INT". Another one is that I forgot to add %prec to the unary operator, which leads to error. Debugging these details spend a lot of time for me, even more than the time to write the code itself :(

## Feedback to T.A.s

I think this project is educative, especially when it is associated with the first project. However, I found that there are some ambiguous instructions in the spec. I suggest that the spec can clearly indicate that real number consists of float and scientific; "type" in array declaration can clearly specify that if the type is array, it is not only a keyword "array", but another "array <integer> of type" instead.
