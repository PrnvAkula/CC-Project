%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern FILE* yyin;
%}

%union {
    char* str;
    int num;
    char chr;
}

%token <str> begin_prog end_prog begin_vardecl end_vardecl
%token <str> keyword
%token <str> name
%token <num> digit
%token <chr> rel_op OB CB SC comma colon OCB CCB OSB CSB
%token <chr> arith_op
%token single_quot
%token NL


%%
start : begin_prog code end_prog { printf("Valid syntax!!!\n"); return 0;}
;
code : vardecl program { }
;

vardecl : begin_vardecl decl end_vardecl { }
;

decl : OB name comma digit CB SC decl { 
            if($4 != 2 && $4 != 8 && $4 != 10){
                yyerror("error: invalid base entered!");
            }
        }
     | { }
;

program: var_assign { }
       | print_scan { }
       | cond_stmt { }
       | loop_stmt { }
;

var_assign :
;

print_scan :
;

cond_stmt :
;

loop_stmt : 
;

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }
    yyparse();
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    printf("Error: %s\n", s);
    exit(1);
}