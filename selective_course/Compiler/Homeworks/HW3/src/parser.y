%{
#include "AST/AstDumper.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/expression.hpp"
#include "AST/for.hpp"
#include "AST/function.hpp"
#include "AST/if.hpp"
#include "AST/print.hpp"
#include "AST/program.hpp"
#include "AST/read.hpp"
#include "AST/return.hpp"
#include "AST/variable.hpp"
#include "AST/while.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <typeinfo>

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

extern uint32_t line_num;   /* declared in scanner.l */
extern char current_line[]; /* declared in scanner.l */
extern FILE *yyin;          /* declared by lex */
extern char *yytext;        /* declared by lex */

static AstNode *root;

extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}

%code requires {
    #include <vector>
    using namespace std;
    class AstNode;
    class CompoundStatementNode;
    class DeclNode;
    class FunctionNode;
    class VariableNode;
    class ConstantValueNode;
    class AssignmentNode;
    class BinaryOperatorNode;
    class WhileNode;
    class ForNode;
    class ReturnNode;
    class FunctionInvocationNode;
    class ExpressionNode;
    class VariableReferenceNode;
    class IfNode;
}

    /* For yylval */
%union {
    /* basic semantic value */
    char *identifier;
    int integer;
    float real;

    AstNode *node;
    DeclNode *decl_ptr;
    ConstantValueNode *const_val_ptr;
    CompoundStatementNode *compound_stmt_ptr;
    FunctionNode *func_ptr;
    AstNode *ast_ptr;
    ExpressionNode *exp_ptr;
    FunctionInvocationNode *func_call_ptr;
    VariableReferenceNode *var_ref_ptr;
    IfNode *if_ptr;
    WhileNode *while_ptr;
    ForNode *for_ptr;
    ReturnNode *ret_ptr;

    vector<DeclNode*> *decls_ptr;
    vector<FunctionNode*> *funcs_ptr;
    vector<VariableNode*> *vars_ptr;
    vector<AstNode*> *asts_ptr;
    vector<ExpressionNode*> *exps_ptr;
};

%type <identifier> ProgramName ID Type INTEGER REAL STRING BOOLEAN ScalarType STRING_LITERAL ArrType ArrDecl FunctionName ReturnType
%type <compound_stmt_ptr> CompoundStatement ElseOrNot
%type <decl_ptr> Declaration FormalArg
%type <const_val_ptr> LiteralConstant StringAndBoolean IntegerAndReal
%type <integer> INT_LITERAL NegOrNot
%type <real> REAL_LITERAL
%type <func_ptr> Function FunctionDeclaration FunctionDefinition
%type <ast_ptr> Statement Simple
%type <exp_ptr> Expression
%type <func_call_ptr> FunctionInvocation FunctionCall
%type <var_ref_ptr> VariableReference
%type <if_ptr> Condition
%type <while_ptr> While
%type <for_ptr> For
%type <ret_ptr> Return

%type <decls_ptr> DeclarationList Declarations FormalArgs FormalArgList
%type <funcs_ptr> FunctionList Functions
%type <vars_ptr> IdList
%type <asts_ptr> StatementList Statements
%type <exps_ptr> ExpressionList Expressions ArrRefList ArrRefs

    /* Follow the order in scanner.l */

    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token ASSIGN
%left OR
%left AND
%right NOT
%left LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right UNARY_MINUS

    /* Keyword */
%token ARRAY BOOLEAN INTEGER REAL STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token FALSE TRUE
%token PRINT READ

    /* Identifier */
%token ID

    /* Literal */
%token INT_LITERAL
%token REAL_LITERAL
%token STRING_LITERAL

%%

ProgramUnit:
    Program
    |
    Function
;

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        root = new ProgramNode(@1.first_line, @1.first_column,
                               $1, *$3, *$4, $5);

        free($1);
    }
;

ProgramName:
    ID
;

DeclarationList:
    Epsilon {
        $$ = new vector<DeclNode*>();
    }
    |
    Declarations
;

Declarations:
    Declaration {
        $$ = new vector<DeclNode*> ({$1});
    }
    |
    Declarations Declaration {
        $1->push_back($2);
        $$ = $1;
    }
;

FunctionList:
    Epsilon {
        $$ = new vector<FunctionNode*>();
    }
    |
    Functions
;

Functions:
    Function {
        $$ = new vector<FunctionNode*>{$1};
    }
    |
    Functions Function {
        $1->push_back($2);
        $$ = $1;
    }
;

Function:
    FunctionDeclaration
    |
    FunctionDefinition
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON {
        $$ = new FunctionNode(@1.first_line, @1.first_column, $1, *$3, $5, nullptr);
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END {
        $$ = new FunctionNode(@1.first_line, @1.first_column, $1, *$3, $5, $6);
    }
;

FunctionName:
    ID
;

FormalArgList:
    Epsilon {
        $$ = new vector<DeclNode*>();
    }
    |
    FormalArgs
;

FormalArgs:
    FormalArg {
        $$ = new vector<DeclNode*>({$1});
    }
    |
    FormalArgs SEMICOLON FormalArg {
        $1->push_back($3);
        $$ = $1;
    }
;

FormalArg:
    IdList COLON Type {
        $$ = new DeclNode(@1.first_line, @1.first_column, *$1, $3);
    }
;

IdList:
    ID {
        $$ = new vector<VariableNode*>({
            new VariableNode(@1.first_line, @1.first_column, $1)
        });
    }
    |
    IdList COMMA ID {
        $1->push_back(new VariableNode(@3.first_line, @3.first_column, $3));
        $$ = $1;
    }
;

ReturnType:
    COLON ScalarType {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = (char*)"void";
    }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        $$ = new DeclNode(@1.first_line, @1.first_column, *$2, $4);
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        $$ = new DeclNode(@1.first_line, @1.first_column, *$2, $4);
    }
;

Type:
    ScalarType
    |
    ArrType
;

ScalarType:
    INTEGER {
        $$ = (char*)"integer";
    }
    |
    REAL {
        $$ = (char*)"real";
    }
    |
    STRING {
        $$ = (char*)"string";
    }
    |
    BOOLEAN {
        $$ = (char*)"boolean";
    }
;

ArrType:
    ArrDecl ScalarType {
      string s($2);
      string decl($1);
      s += " " + decl;
      $$ = (char*)s.c_str();
    }
;

ArrDecl:
    ARRAY INT_LITERAL OF {
        string s = to_string($2);
        s = '[' + s + ']';
        $$ = (char*)s.c_str();
    }
    |
    ArrDecl ARRAY INT_LITERAL OF {
        string s($1);
        s += '[' + to_string($3) + ']';
        $$ = (char*)s.c_str();
    }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        if($1==1){
            $$ = new ConstantValueNode(@2.first_line, @2.first_column, $1*$2);
        }
        else{
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1*$2);
        }
    }
    |
    NegOrNot REAL_LITERAL {
        if($1==1){
            $$ = new ConstantValueNode(@2.first_line, @2.first_column, $1*$2);
        }
        else{
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1*$2);
        }
    }
    |
    StringAndBoolean
;

NegOrNot:
    Epsilon {
        $$ = 1;
    }
    |
    MINUS %prec UNARY_MINUS {
        $$ = -1;
    }
;

StringAndBoolean:
    STRING_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
    |
    TRUE {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, true);
    }
    |
    FALSE {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, false);
    }
;

IntegerAndReal:
    INT_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
    |
    REAL_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
;

    /*
       Statements
                  */

Statement:
    CompoundStatement {
        $$ = static_cast<AstNode*>($1);
    }
    |
    Simple
    |
    Condition {
        $$ = static_cast<AstNode*>($1);
    }
    |
    While {
        $$ = static_cast<AstNode*>($1);
    }
    |
    For {
        $$ = static_cast<AstNode*>($1);
    }
    |
    Return {
        $$ = static_cast<AstNode*>($1);
    }
    |
    FunctionCall {
        $$ = static_cast<AstNode*>($1);
    }
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END {
        $$ = new CompoundStatementNode(@1.first_line, @1.first_column, *$2, *$3);
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON {
        $$ = new AssignmentNode(@2.first_line, @2.first_column, $1, $3);
    }
    |
    PRINT Expression SEMICOLON {
        $$ = new PrintNode(@1.first_line, @1.first_column, $2);
    }
    |
    READ VariableReference SEMICOLON {
        $$ = new ReadNode(@1.first_line, @1.first_column, $2);
    }
;

VariableReference:
    ID ArrRefList {
        $$ = new VariableReferenceNode(@1.first_line, @1.first_column, $1, *$2);
    }
;

ArrRefList:
    Epsilon {
        $$ = new vector<ExpressionNode*>();
    }
    |
    ArrRefs
;

ArrRefs:
    L_BRACKET Expression R_BRACKET {
        $$ = new vector<ExpressionNode*>({$2});
    }
    |
    ArrRefs L_BRACKET Expression R_BRACKET {
        $1->push_back($3);
        $$ = $1;
    }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF {
        $$ = new IfNode(@1.first_line, @1.first_column, $2, $4, $5);
    }
;

ElseOrNot:
    ELSE
    CompoundStatement {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = nullptr;
    }
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO {
        $$ = new WhileNode(@1.first_line, @1.first_column, $2, $4);
    }
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO {
        vector<VariableNode*> var_list = {new VariableNode(@2.first_line, @2.first_column, $2)};
        DeclNode *decl = new DeclNode(@2.first_line, @2.first_column, var_list, (char*)"integer");

        VariableReferenceNode *var_ref = new VariableReferenceNode(@2.first_line, @2.first_column, $2, {});
        ExpressionNode *const_val_assign = new ConstantValueNode(@4.first_line, @4.first_column, $4);
        AssignmentNode *assign = new AssignmentNode(@3.first_line, @3.first_column, var_ref, const_val_assign);
        
        ConstantValueNode *const_val = new ConstantValueNode(@6.first_line, @6.first_column, $6);
        $$ = new ForNode(@1.first_line, @1.first_column, decl, assign, const_val, $8);
    }
;

Return:
    RETURN Expression SEMICOLON {
        $$ = new ReturnNode(@1.first_line, @1.first_column, $2);
    }
;

FunctionCall:
    FunctionInvocation SEMICOLON
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS {
        $$ = new FunctionInvocationNode(@1.first_line, @1.first_column, $1, *$3);
    }
;

ExpressionList:
    Epsilon {
        $$ = new vector<ExpressionNode*>();
    }
    |
    Expressions
;

Expressions:
    Expression {
        $$ = new vector<ExpressionNode*>({$1});
    }
    |
    Expressions COMMA Expression {
        $1->push_back($3);
        $$ = $1;
    }
;

StatementList:
    Epsilon {
        $$ = new vector<AstNode*>();
    } 
    |
    Statements
;

Statements:
    Statement {
        $$ = new vector<AstNode*>({$1});
    }
    |
    Statements Statement {
        $1->push_back($2);
        $$ = $1;
    }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS {
        $$ = $2;
    }
    |
    MINUS Expression %prec UNARY_MINUS {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column, "neg", $2);
    }
    |
    Expression MULTIPLY Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "*", $1, $3);
    }
    |
    Expression DIVIDE Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "/", $1, $3);
    }
    |
    Expression MOD Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "mod", $1, $3);
    }
    |
    Expression PLUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "+", $1, $3);
    }
    |
    Expression MINUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "-", $1, $3);
    }
    |
    Expression LESS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "<", $1, $3);
    }
    |
    Expression LESS_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "<=", $1, $3);
    }
    |
    Expression GREATER Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, ">", $1, $3);
    }
    |
    Expression GREATER_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, ">=", $1, $3);
    }
    |
    Expression EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "=", $1, $3);
    }
    |
    Expression NOT_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "<>", $1, $3);
    }
    |
    NOT Expression {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column, "not", $2);
    }
    |
    Expression AND Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "and", $1, $3);
    }
    |
    Expression OR Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "or", $1, $3);
    }
    |
    IntegerAndReal {
        $$ = static_cast<ExpressionNode*>($1);
    }
    |
    StringAndBoolean {
        $$ = static_cast<ExpressionNode*>($1);
    }
    |
    VariableReference {
        $$ = static_cast<ExpressionNode*>($1);
    }
    |
    FunctionInvocation {
        $$ = static_cast<ExpressionNode*>($1);
    }
;

    /*
       misc
            */
Epsilon:
;

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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename> [--dump-ast]\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed");
        exit(-1);
    }

    yyparse();

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        AstDumper ast_dumper;
        root->accept(ast_dumper);
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
