%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "part2.h"  // Include AST header

extern FILE* yyin;
extern FILE* yyout;
void yyerror(const char* s);
int yylex(void);

// Root of AST
ASTNode* program_root = NULL;
%}

%union {
    char* str;
    int num;
    char chr;

    struct {
        int int_val;
        int base;
        char* var_name;
        char char_val;
        ASTNode* ast;  // Add AST node pointer
    } t;
    
    ASTNode* node;  // Generic AST node pointer
}

%token <str> begin_prog end_prog begin_vardecl end_vardecl
%token <str> keyword type IF ELSE end begin scan print FOR WHILE INC DEC DO TO
%token <str> ID
%token <num> digit
%token <chr> rel_op OB CB SC comma colon OCB CCB OSB CSB EQ AT
%token <chr> arith_op
%token single_quot
%token <str> STRING        /* "…" literal */

%nonassoc IFX
%nonassoc ELSE

/* Fix precedence to resolve shift/reduce conflicts */
%left rel_op       /* Add precedence for relational operators */
%left arith_op     /* Arithmetic has higher precedence than relational */

%type <t> expr
%type <node> start code vardecl decl_list decl stmt stmts var_assign print_scan cond_stmt loop_stmt block

%%
start : begin_prog code end_prog { 
        printf("Valid syntax!!!\n"); 
        program_root = $2;
        
        //fprintf(yyout, "AST in the generalized list form\n");
        
        FILE *saved_stdout = stdout;
        stdout = yyout;
        
        printAST(program_root);
        fprintf(yyout, "\n");
        
        stdout = saved_stdout;
        
        printf("AST output written to output.txt\n");
        return 0;
    }
;

code : vardecl stmts { 
        ASTNode* program = createProgramNode();
        
        // Add all variable declarations to the program
        ASTNode* current_decl = $1;
        while (current_decl) {
            addStatementToBlock(program, current_decl);
            current_decl = current_decl->next;
        }
        
        // Add all statements to the program
        ASTNode* current_stmt = $2;
        while (current_stmt) {
            addStatementToBlock(program, current_stmt);
            current_stmt = current_stmt->next;
        }
        
        $$ = program;
    }
;

vardecl : begin_vardecl decl_list end_vardecl { $$ = $2; }
;

decl_list: decl {
        $$ = $1;
    }
    | decl_list decl { 
        // Link declarations together
        ASTNode* last = $1;
        while (last->next) {
            last = last->next;
        }
        last->next = $2;
        $$ = $1;
    }
;

decl : OB ID opt_array comma type CB SC { 
        $$ = createVarDeclNode($2, $5);
    }
;

opt_array: OSB digit CSB {}
        | {}
;

stmts : stmt { 
        $$ = $1;
    }
    | stmts stmt { 
        // Link statements together
        ASTNode* last = $1;
        while (last->next) {
            last = last->next;
        }
        last->next = $2;
        $$ = $1;
    }
;

block: begin stmts end {
        ASTNode* block_node = createBlockNode();
        ASTNode* current = $2;
        while (current) {
            addStatementToBlock(block_node, current);
            current = current->next;
        }
        $$ = block_node;
    }
;

stmt: var_assign { $$ = $1; }
    | print_scan { $$ = $1; }
    | cond_stmt { $$ = $1; }
    | loop_stmt { $$ = $1; }
;

var_assign: ID EQ expr SC {
        ASTNode* var_node = createVariableNode($1);
        $$ = createAssignNode(var_node, $3.ast);
    }
;

print_scan: print OB STRING comma expr CB SC {
        $$ = createPrintNode($3, $5.ast);
    }
    | scan OB ats CB SC {
        // For now, create a simple print node with null value
        $$ = createPrintNode("scan placeholder", NULL);
    }
;

ats : AT comma ats { }
    | AT { }
;

cond_stmt: IF OB expr CB block SC %prec IFX {
        $$ = createIfNode($3.ast, $5, NULL);
    }
    | IF OB expr CB block ELSE block SC {
        $$ = createIfNode($3.ast, $5, $7);
    }
;

loop_stmt: FOR ID EQ expr TO expr INC expr DO block SC {
        // Create init assignment
        ASTNode* var_node = createVariableNode($2);
        ASTNode* init = createAssignNode(var_node, $4.ast);
        
        // Create for node with proper components
        $$ = createForNode(init, $6.ast, $8.ast, $10);
    }
    | FOR ID EQ expr TO expr DEC expr DO block SC {
        // Similar to above but with dec instead of inc
        ASTNode* var_node = createVariableNode($2);
        ASTNode* init = createAssignNode(var_node, $4.ast);
        
        // Create for node with proper components
        $$ = createForNode(init, $6.ast, $8.ast, $10);
    }
    | WHILE OB expr CB DO block SC {
        // Create a while loop (using for node structure)
        $$ = createForNode(NULL, $3.ast, NULL, $6);
    }
;

expr: OB expr CB {
        /* parentheses: propagate the inner node */
        $$ = $2;
    }
    | digit comma digit {
        /* literal (n,base) */
        $$.int_val = $1;
        $$.base = $3;
        $$.var_name = NULL;
        $$.ast = createNumberNode($1, $3);
    }
    | ID {
        /* variable reference */
        $$.var_name = $1;
        $$.ast = createVariableNode($1);
    }
    | expr arith_op expr {
        /* binary operator */
        NodeType op_type;
        switch($2) {
            case '+': op_type = NODE_ADD; break;
            case '-': op_type = NODE_SUB; break;
            case '*': op_type = NODE_MUL; break;
            case '/': op_type = NODE_DIV; break;
            case '%': op_type = NODE_MOD; break;
            default: op_type = NODE_ADD; break; // Default to add
        }
        
        $$.int_val = $1.int_val;
        $$.char_val = $2;
        $$.base = $3.int_val;
        $$.ast = createOperatorNode(op_type, $1.ast, $3.ast);
    }
    | expr rel_op expr {  /* Changed from ID rel_op expr to expr rel_op expr */
        /* relational operator */
        NodeType op_type;
        switch($2) {
            case '<': op_type = NODE_LESS; break;
            case '>': op_type = NODE_GREATER; break;
            case '=': op_type = NODE_EQUAL; break;
            default: op_type = NODE_EQUAL; break;
        }
        
        $$.ast = createOperatorNode(op_type, $1.ast, $3.ast);
    }
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

    yyout = fopen("output.txt", "w");
    if (!yyout) {
        perror("Error opening output file");
        fclose(yyin);
        return 1;
    }

    yyparse();
    
    // Free the AST
    if (program_root) {
        freeAST(program_root);
    }
    
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    printf("Error: %s\n", s);
    exit(1);
}