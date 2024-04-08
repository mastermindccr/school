%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern int32_t line_num;    /* declared in scanner.l */
extern char current_line[]; /* declared in scanner.l */
extern FILE *yyin;          /* declared by lex */
extern char *yytext;        /* declared by lex */

extern int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}

%token ASSIGN COMP
%token AND OR NOT
%token VAR OF DEF RETURN
%token SCALAR_TYPE ARRAY BOOL INT REAL STRING ID
%token BEG END
%token WHILE DO IF THEN ELSE FOR TO
%token PRINT READ

%right '='
%left OR
%left AND
%right NOT
%left COMP
%left '+' '-'
%left '*' '/' '%'

%start ProgramUnit

%%

ProgramUnit: Program
           | FUNC
           ;

Program: ID ';'
         VAR_CONST_DECLS
         FUNCS
         COMP_STMT
         END
       ;

VAR_CONST_DECLS: VAR_DECL VAR_CONST_DECLS
               | CONST_DECL VAR_CONST_DECLS
               |
               ;

VAR_DECL: VAR ID_LIST ':' SCALAR_TYPE ';'
        | VAR ID_LIST ':' ARRAY_TYPE ';'
        ;

ARRAY_TYPE: ARRAY INT OF TYPE ;

TYPE: SCALAR_TYPE | ARRAY_TYPE ;

ID_LIST: ID ',' ID_LIST
       | ID
       ;

CONST_DECL: VAR ID_LIST ':' SCALAR ';' ;

SCALAR: INT | '+' INT | '-' INT
      | REAL | '+' REAL | '-' REAL
      | STRING
      | BOOL
      ;

FUNCS: FUNC FUNCS
     |
     ;

FUNC: FUNC_DECL
    | FUNC_DEF;

FUNC_DECL: FUNC_INTERFACE ':' SCALAR_TYPE ';'
         | FUNC_INTERFACE ';'
         ;

FUNC_INTERFACE: ID '(' ARGS ')';

ARGS:
    | ARGS1
    ;

ARGS1: ARG ';' ARGS1
     | ARG 
     ;

ARG: ID_LIST ':' TYPE ;

FUNC_DEF: FUNC_INTERFACE ':' SCALAR_TYPE
          COMP_STMT
          END
        | FUNC_INTERFACE
          COMP_STMT
          END
        ;

STMTS:
     | STMTS1
     ;

STMTS1: STMT STMTS1
      | STMT
      ;

STMT: COMP_STMT
    | SIMPLE_STMT
    | COND_STMT
    | WHILE_STMT
    | FOR_STMT
    | RETURN_STMT
    | FUNC_CALL
    ;

COMP_STMT: BEG
           VAR_CONST_DECLS
           STMTS
           END
         ;

SIMPLE_STMT: VAR_REF ASSIGN EXPR ';'
           | PRINT EXPR ';'
           | READ VAR_REF ';'
           ;

COND_STMT: IF EXPR THEN
           COMP_STMT
           ELSE
           COMP_STMT
           END IF
         | IF EXPR THEN
           COMP_STMT
           END IF
         ;

WHILE_STMT: WHILE EXPR DO
            COMP_STMT
            END DO
          ;

FOR_STMT: FOR ID ASSIGN INT TO INT DO
          COMP_STMT
          END DO
        ;

RETURN_STMT: RETURN EXPR ';' ;

FUNC_CALL: ID '(' EXPRS ')' ';' ;

VAR_REF: ID
       | ID EXPRS_WITH_BRACKET
       ;

EXPRS_WITH_BRACKET: EXPR_WITH_BRACKET EXPRS_WITH_BRACKET
                  | EXPR_WITH_BRACKET
                  ;

EXPR_WITH_BRACKET: '[' EXPR ']' ;

EXPRS: 
     | EXPRS1
     ;

EXPRS1: EXPR ',' EXPRS1
      | EXPR
      ;

EXPR: SCALAR
    | VAR_REF
    | ID '(' EXPRS ')'
    | NOT EXPR
    | EXPR AND EXPR
    | EXPR OR EXPR
    | EXPR COMP EXPR
    | EXPR OP EXPR
    | '-' EXPR %prec '*'
    | '(' EXPR ')' %prec '*'
    ;

OP: '+' | '-' | '*' | '/' | '%' ;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, current_line, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed");
        exit(-1);
    }

    yyparse();

    fclose(yyin);
    yylex_destroy();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
