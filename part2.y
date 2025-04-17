%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern FILE* yyin;
void yyerror(const char* s);
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
%token <str> keyword IF ELSE end begin scan print FOR WHILE INC DEC DO TO
%token <str> name
%token <num> digit
%token <chr> rel_op OB CB SC comma colon OCB CCB OSB CSB EQ AT
%token <chr> arith_op
%token single_quot double_quot
%token NL

%nonassoc IFX
%nonassoc ELSE

%type <t> var_assign

%%
start : begin_prog code end_prog { printf("Valid syntax!!!\n"); return 0;}
;
code : vardecl stmts { }
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

stmts: var_assign { }
       | print_scan { }
       | cond_stmt { }
       | loop_stmt { }
;

var_assign : name colon EQ OB digit comma digit CB SC { $<t.name>$ = $1; $<t.int_val>$ = $5; $<t.base>$ = $7; }
           | name colon EQ single_quot name single_quot {$<t.name>$ = $1; $<t.char_val>$ = $5;}
;

print_scan : print OB double_quot name double_quot CB SC { }
           | scan OB ats CB SC {}
;

ats : AT comma ats { }
    | AT { }
;

cond_stmt : IF name rel_op OB digit comma digit CB NL begin stmts end %prec IFX{ }
          | IF name EQ EQ OB digit comma digit CB NL begin stmts end %prec IFX{ }
          | IF name rel_op OB digit comma digit CB NL begin stmts end ELSE begin stmts end SC{ }
          | IF name EQ EQ OB digit comma digit CB NL begin stmts end ELSE begin stmts end SC{ }
;

loop_stmt : FOR name colon EQ OB digit comma digit CB TO name arith_op OB digit comma digit CB INC OB digit comma digit CB DO NL begin stmts end SC { }
          | FOR name colon EQ OB digit comma digit CB TO OB digit comma digit CB INC OB digit comma digit CB DO NL begin stmts end SC { }
          | WHILE OB name rel_op OB digit comma digit CB CB DO NL begin stmts end SC { }
          | WHILE OB name EQ EQ OB digit comma digit CB CB DO NL begin stmts end SC { }
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