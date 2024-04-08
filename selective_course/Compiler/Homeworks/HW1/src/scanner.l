%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST_SOURCE(src)                 appendToCurrentLine(src)
#define LIST_TOKEN(name)            do { LIST_SOURCE(yytext); if(opt_tok) printf("<%s>\n", name); } while(0)
#define LIST_LITERAL(name, literal) do { LIST_SOURCE(yytext); if(opt_tok) printf("<%s: %s>\n", name, literal); } while(0)
#define MAX_LINE_LENG               512

uint32_t line_num = 1;
static char current_line[MAX_LINE_LENG];

static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
static char *current_line_ptr = current_line;

static void appendToCurrentLine(const char *yytext_ptr);

%}
    /* exclusive condition */
%x comment 
%%
    /* comment should have indentation */

    /* strings contained in the quotation marks is identified as pure string */
    /* C-style comment */
"/*" {
    LIST_SOURCE(yytext);
    BEGIN comment;
}

<comment>"*/" {
    LIST_SOURCE(yytext);
    BEGIN INITIAL;
}

<comment>. {
    LIST_SOURCE(yytext);
}

<comment>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, current_line);
    }
    ++line_num;
    current_line[0] = '\0';
    current_line_ptr = current_line;
}

    /* C++-style pseudocomment */
"//&"[S|T][+|-].* {
    char option = yytext[3];
    char act = yytext[4];
    if(option=='S'){
        if(act=='+'){
            opt_src = 1;
        }
        else if(act=='-'){
            opt_src = 0;
        }
        else{
            printf("src listing failed, stopped at line %d\n", line_num);
            exit(-1);
        }
    }
    else if(option=='T'){
        if(act=='+'){
            opt_tok = 1;
        }
        else if(act=='-'){
            opt_tok = 0;
        }
        else{
            printf("token listing failed, stopped at line %d\n", line_num);
            exit(-1);
        }
    }
    LIST_SOURCE(yytext);
}

    /* C++-style comment */
"//".* {
    LIST_SOURCE(yytext);
}

    /* multi character token */
mod|:=|<=|<>|>=|and|or|not {
    LIST_TOKEN(yytext);
}

    /* remove space */
[\t ] {
    LIST_SOURCE(yytext);
}

    /* single character token */
    /* hyphen must be put at the end of the rule */
[,;:()\[\]+*/<>=-] {
    LIST_TOKEN(yytext);
}

    /* keyword */
    /* add negative lookbehind and negative lookahead to make sure it matches the exact string */
var|def|array|of|boolean|integer|real|string|true|false|while|do|if|then|else|for|to|begin|end|print|read|return {
    char res[yyleng+3]; // char array should be initialized with one extra space for saving \0 in the end
    memset(res, 0, sizeof(res));
    res[0] = 'K';
    res[1] = 'W';
    memcpy(&res[2], yytext, yyleng);
    LIST_TOKEN(res);
}

    /* identifier */
[a-zA-Z][a-zA-Z0-9]* {
    LIST_LITERAL("id", yytext);
}

    /* floating point constant */
([1-9][0-9]*|[0-9])"."([0-9]*[1-9]|0) {
    LIST_LITERAL("float", yytext);
}

    /* integer constant */
    /* octal integer */
0[0-7]+ {
    LIST_LITERAL("oct_integer", yytext);
}

    /* decimal integer */
[1-9][0-9]*|[0-9] {
    LIST_LITERAL("integer", yytext);
}

    /* scientific notation */
    /* integer start */
([1-9][0-9]*)[e|E][+|-]?([1-9][0-9]*|[0-9]) {
    LIST_LITERAL("scientific", yytext);
}
    /* real number start - exclude 0.xxx */
([1-9][0-9]*)"."([0-9]*[1-9]|0)[e|E][+|-]?([1-9][0-9]*|[0-9]) {
    LIST_LITERAL("scientific", yytext);
}

    /* real number start - 0.xxx */
0"."([0-9]*[1-9])[e|E][+|-]?([1-9][0-9]*|[0-9]) {
    LIST_LITERAL("scientific", yytext);
}

    /* string constant */
\"([^"]|\s|\"\")*\" {
    char tmp[yyleng-1];
    memset(tmp, 0, sizeof(tmp));
    int pos = 0;
    int skip = 0;
    for(int i = 1;i<yyleng-1;i++){
        if(skip){
            skip = 0;
            continue;
        }
        if(i+1!=yyleng-1 && yytext[i]=='"' && yytext[i+1]=='"'){
            skip = 1;
        }
        tmp[pos] = yytext[i];
        pos++;
    }
    LIST_LITERAL("string", tmp);
}

\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, current_line);
    }
    ++line_num;
    current_line[0] = '\0';
    current_line_ptr = current_line;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void appendToCurrentLine(const char *yytext_ptr) {
    while (*yytext_ptr) {
        *current_line_ptr = *yytext_ptr;
        ++current_line_ptr;
        ++yytext_ptr;
    }
    *current_line_ptr = '\0';
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    fclose(yyin);
    /* Free resources used by the scanner; otherwise, unused data structures
    will persist throughout the successive compiler phases. */
    yylex_destroy();

    return 0;
}
