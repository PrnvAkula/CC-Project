#include "part2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a number node with base
ASTNode* createNumberNode(int value, int base) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_NUMBER;
    node->data.number.value = value;
    node->data.number.base = base;
    node->next = NULL;
    return node;
}

// Create a variable reference node
ASTNode* createVariableNode(char* name) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_VARIABLE;
    node->data.name = strdup(name);
    node->next = NULL;
    return node;
}

// Create a string literal node
ASTNode* createStringNode(char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_STRING;
    node->data.name = strdup(value);
    node->next = NULL;
    return node;
}

// Create a binary operator node
ASTNode* createOperatorNode(NodeType type, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->data.op.left = left;
    node->data.op.right = right;
    node->next = NULL;
    return node;
}

// Create an assignment node
ASTNode* createAssignNode(ASTNode* variable, ASTNode* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_ASSIGN;
    node->data.assign.variable = variable;
    node->data.assign.value = value;
    node->next = NULL;
    return node;
}

// Create a variable declaration node
ASTNode* createVarDeclNode(char* name, char* type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_VAR_DECL;
    node->data.decl.name = strdup(name);
    node->data.decl.type_name = strdup(type);
    node->next = NULL;
    return node;
}

// Create a print statement node
ASTNode* createPrintNode(char* format, ASTNode* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_PRINT;
    node->data.print.format = strdup(format);
    node->data.print.value = value;
    node->next = NULL;
    return node;
}

// Create an if statement node
ASTNode* createIfNode(ASTNode* condition, ASTNode* if_body, ASTNode* else_body) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_IF;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.if_body = if_body;
    node->data.if_stmt.else_body = else_body;
    node->next = NULL;
    return node;
}

// Create a for loop node
ASTNode* createForNode(ASTNode* init, ASTNode* condition, ASTNode* update, ASTNode* body) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_FOR;
    node->data.for_loop.init = init;
    node->data.for_loop.condition = condition;
    node->data.for_loop.update = update;
    node->data.for_loop.body = body;
    node->next = NULL;
    return node;
}

// Create a block node for multiple statements
ASTNode* createBlockNode() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_BLOCK;
    node->data.block.statements = NULL;
    node->data.block.count = 0;
    node->next = NULL;
    return node;
}

// Add a statement to a block node
void addStatementToBlock(ASTNode* block, ASTNode* statement) {
    if (block->type != NODE_BLOCK && block->type != NODE_PROGRAM) {
        fprintf(stderr, "Error: Not a block or program node\n");
        return;
    }
    
    block->data.block.count++;
    block->data.block.statements = (ASTNode**)realloc(
        block->data.block.statements, 
        block->data.block.count * sizeof(ASTNode*)
    );
    
    if (!block->data.block.statements) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    block->data.block.statements[block->data.block.count - 1] = statement;
}

// Create a program node (top-level node)
ASTNode* createProgramNode() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_PROGRAM;
    node->data.block.statements = NULL;
    node->data.block.count = 0;
    node->next = NULL;
    return node;
}

// Print the AST in generalized list format
void printAST(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            printf("(\n");
            for (int i = 0; i < node->data.block.count; i++) {
                printAST(node->data.block.statements[i]);
                if (i < node->data.block.count - 1) {
                    printf("\n");
                }
            }
            printf("\n)");
            break;
            
        case NODE_NUMBER:
            printf("(%d %d)", node->data.number.value, node->data.number.base);
            break;
            
        case NODE_VARIABLE:
            printf("%s", node->data.name);
            break;
            
        case NODE_STRING:
            printf("\"%s\"", node->data.name);
            break;
            
        case NODE_ADD:
            printf("(+ ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_SUB:
            printf("(- ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_MUL:
            printf("(* ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_DIV:
            printf("(/ ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_MOD:
            printf("%% ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_GREATER:
            printf("(> ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_LESS:
            printf("(< ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_EQUAL:
            printf("(== ");
            printAST(node->data.op.left);
            printf(" ");
            printAST(node->data.op.right);
            printf(")");
            break;
            
        case NODE_ASSIGN:
            printf("(:= ");
            printAST(node->data.assign.variable);
            printf(" ");
            printAST(node->data.assign.value);
            printf(")");
            break;
            
        case NODE_VAR_DECL:
            printf("(%s %s)", node->data.decl.name, node->data.decl.type_name);
            break;
            
        case NODE_PRINT:
            printf("(print %s ", node->data.print.format);
            printAST(node->data.print.value);
            printf(")");
            break;
            
        case NODE_IF:
            printf("(if\n");
            printAST(node->data.if_stmt.condition);
            printf("\n");
            printAST(node->data.if_stmt.if_body);
            if (node->data.if_stmt.else_body) {
                printf("\n");
                printAST(node->data.if_stmt.else_body);
            }
            printf("\n)");
            break;
            
        case NODE_FOR:
            printf("(for\n");
            printAST(node->data.for_loop.init);
            printf("\n");
            printAST(node->data.for_loop.condition);
            printf("\n");
            printAST(node->data.for_loop.update);
            printf("\n");
            printAST(node->data.for_loop.body);
            printf("\n)");
            break;
            
        default:
            printf("Unknown node type");
    }
}

// Free all memory allocated for the AST
void freeAST(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_VARIABLE:
        case NODE_STRING:
            free(node->data.name);
            break;
            
        case NODE_ADD:
        case NODE_SUB:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_GREATER:
        case NODE_LESS:
        case NODE_EQUAL:
            freeAST(node->data.op.left);
            freeAST(node->data.op.right);
            break;
            
        case NODE_ASSIGN:
            freeAST(node->data.assign.variable);
            freeAST(node->data.assign.value);
            break;
            
        case NODE_VAR_DECL:
            free(node->data.decl.name);
            free(node->data.decl.type_name);
            break;
            
        case NODE_PRINT:
            free(node->data.print.format);
            freeAST(node->data.print.value);
            break;
            
        case NODE_IF:
            freeAST(node->data.if_stmt.condition);
            freeAST(node->data.if_stmt.if_body);
            if (node->data.if_stmt.else_body) {
                freeAST(node->data.if_stmt.else_body);
            }
            break;
            
        case NODE_FOR:
            freeAST(node->data.for_loop.init);
            freeAST(node->data.for_loop.condition);
            freeAST(node->data.for_loop.update);
            freeAST(node->data.for_loop.body);
            break;
            
        case NODE_PROGRAM:
        case NODE_BLOCK:
            for (int i = 0; i < node->data.block.count; i++) {
                freeAST(node->data.block.statements[i]);
            }
            free(node->data.block.statements);
            break;
    }
    
    free(node);
}