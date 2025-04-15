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

    struct {
        int int_val;
        int base;
        char* name;
        char char_val;
    }t;
}

%token <str> begin_prog end_prog begin_vardecl end_vardecl
%token <str> keyword
%token <str> name
%token <num> digit
%token <chr> rel_op OB CB SC comma colon OCB CCB OSB CSB EQ AT
%token <chr> arith_op
%token single_quot double_quot
%token NL

%type <t> var_assign

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

var_assign : name SC EQ OB digit comma digit CB SC { $<t.name>$ = $1; $<t.int_val>$ = $5; $<t.base>$ = $7; }
           | name SC EQ single_quot name single_quot {$<t.name>$ = $1; $<t.char_val>$ = $5;}
;

print_scan : print OB double_quot name double_quot CB SC { }
           | scan OB ats CB SC { }
;

ats : AT comma ats { }
    | AT { }
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