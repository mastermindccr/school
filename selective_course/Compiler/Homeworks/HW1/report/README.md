# hw1 report

|Field|Value|
|-:|:-|
|Name|陳星宇|
|ID|109550060|

## How much time did you spend on this project

7 hours

## Project overview

## Macros
### LIST_SOURCE(src)
This function saves the parameter in a pointer, when `\n` is encountered and src listing option is turned on, the line saved in the pointer will be printed out and the pointer is reinitialized.

### LIST_TOKEN(tok)
This function prints out the token with it squeezed by the angle brackets.

### LIST_LITERAL(name, lit)
This function prints out the literal with its category in the form of `<category>: literal`. The category includes integer, oct_integer, string, etc.

## Tokens
### Delimiter
If any of `,;:()[]` is scanned, use `LIST_TOKEN` to list out the token and use `LIST_SOURCE` to save them in the pointer.

### Arithmetic, Relational and Logical Operators
#### Multi-character Token
If any of `:=|<=|<>|>=|mod|and|or|not` is scanned, use `LIST_TOKEN` to list out the token and use `LIST_SOURCE` to save them in the pointer.
#### Single-character Token
If any of `+-*/<>=` is scanned, use `LIST_TOKEN` to list out the token and use `LIST_SOURCE` to save them in the pointer.

### Reserved Words
If any of the keyword is scanned, put `KW` in the beginning of the scanned text then pass it into `LIST_TOKEN`, and use `LIST_SOURCE` to save them in the pointer.

### Identifiers
If any of the word matches the pattern `[a-zA-z][a-zA-z0-9]*`, it is treated as an identifier, then it will be passed into `LIST_LITERAL`, and use `LIST_SOURCE` to save them in the pointer.

### Integer Constants
#### Octal Integers
If any of the word matches the pattern `0[0-7]+` (use plus here as 0 is viewed as decimal integer), it is treated as an octal integer, then it will be passed into `LIST_LITERAL`, and use `LIST_SOURCE` to save them in the pointer.
#### Decimal Integers
If any of the word matches the pattern `[1-9][0-9]*|[0-9]` (indicates the integer started with non zero if it has more than 1 digit or only has one digit), it is treated as an decimal integer, then it will be passed into `LIST_LITERAL`, and use `LIST_SOURCE` to save them in the pointer.
#### Floating-Point Constants
##### Integer based
If any of the word matches the pattern `([1-9][0-9]*)[e|E][+|-]?([1-9][0-9]*|[0-9])` (0 is excluded from the base), it is treated as a scientific notation, then it will be passed into `LIST_LITERAL`, and use `LIST_SOURCE` to save them in the pointer.
##### Real number based
###### not 0.xxx based
If any of the word matches the pattern `([1-9][0-9]*)"."([0-9]*[1-9]|0)[e|E][+|-]?([1-9][0-9]*|[0-9])`, it is treated as a scientific notation, then it will be passed into `LIST_LITERAL`, and use `LIST_SOURCE` to save them in the pointer.
###### 0.xxx based
If any of the word matches the pattern `0"."([0-9]*[1-9])[e|E][+|-]?([1-9][0-9]*|[0-9])`, it is treated as a scientific notation, then it will be passed into `LIST_LITERAL`, and use `LIST_SOURCE` to save them in the pointer.
```
The reason I divide it into 0 integer based real number and not is to eliminate 0.0 based notation, which is illegal.
```

### String Constants
If any of the word matches the pattern `\"([^"]|\s|\"\")*\"`(`\"\"` is to scan two consecutive double quotes in the input), when two consecutive double quotes are scanned, only record one into LIST_LITERAL but still pass the original yytext into LIST_SOURCE.

### Whitespace Characters
`[\t\s];` discard all whitespaces and tabs. `\n` is contained in another rule as it needs to handle source listing.

### Comments
**Comments will not be parsed into `LIST_TOEKN` or `LIST_LITERAL`**
#### C style Comment
+ Use exlusive start condition `%x` to define a new condition called `comment`.
```
Note that there should be *no whitespace* between the rule and the name of condition.
```
+ When `/*` is scanned, enter the condition `comment` by `BEGIN comment`.
+ Inside the condition, whenever a character is scanned(`\n` excluded), pass it into LIST_SOURCE. When `\n` is read, print out the line and reset the pointer as the normal `\n` rule does.
+ When `*/` is scanned, leave the condition by `BEGIN INITIAL`.
#### C++ style Comment
If any of the word matches `"//".*`, the whole line is treated as a C++-style comment, and the whole line will only be passed into LIST_SOURCE.
### Pseudocomments
If any of the word matches `"//&"[S|T][+|-].*`, the pseudocomments is scanned, and the remaining words will be ignored and only be passed into LIST_SOURCE.
#### Token listing
If `S+` is scanned, `opt_src = 1`, while `S-` indicates `opt_src = 0`.
If `T+` is scanned, `opt_tok = 1`, while `T-` indicates `opt_tok = 0`. 


## What is the hardest you think in this project

Write C style comment using `%x` to declare a condition, as it is different from other rules, which simply need to write down the correct pattern and proceed yytext.

## Feedback to T.A.s

It is nice for TAs to answer the question on the Github at once, thanks for TAs' help!
