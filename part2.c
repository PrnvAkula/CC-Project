#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "part2.h"

// Create a node of specified type
ASTNode* createNode(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->next = NULL;
    return node;
}

// Create a program node (root of AST)
ASTNode* createProgramNode() {
    ASTNode* node = createNode(NODE_PROGRAM);
    node->data.block.count = 0;
    node->data.block.capacity = 10;  // Initial capacity
    node->data.block.statements = (ASTNode**)malloc(10 * sizeof(ASTNode*));
    return node;
}

// Create a block node (for compound statements)
ASTNode* createBlockNode() {
    ASTNode* node = createNode(NODE_BLOCK);
    node->data.block.count = 0;
    node->data.block.capacity = 10;  // Initial capacity
    node->data.block.statements = (ASTNode**)malloc(10 * sizeof(ASTNode*));
    return node;
}

// Add a statement to a block node
void addStatementToBlock(ASTNode* block, ASTNode* statement) {
    if (block->type != NODE_BLOCK && block->type != NODE_PROGRAM) return;
    
    if (block->data.block.count >= block->data.block.capacity) {
        block->data.block.capacity *= 2;
        block->data.block.statements = (ASTNode**)realloc(block->data.block.statements, 
                                          block->data.block.capacity * sizeof(ASTNode*));
    }
    
    block->data.block.statements[block->data.block.count++] = statement;
}

// Create a variable declaration node
ASTNode* createVarDeclNode(char* name, char* type_name) {
    ASTNode* node = createNode(NODE_VAR_DECL);
    node->data.decl.name = strdup(name);
    node->data.decl.type_name = strdup(type_name);
    return node;
}

// Create a variable reference node
ASTNode* createVariableNode(char* name) {
    ASTNode* node = createNode(NODE_VAR);
    node->data.var.name = strdup(name);
    return node;
}

// Create a number literal node
ASTNode* createNumberNode(int value, int base) {
    ASTNode* node = createNode(NODE_NUMBER);
    node->data.num.value = value;
    node->data.num.base = base;
    return node;
}

// Create an assignment node
ASTNode* createAssignNode(ASTNode* variable, ASTNode* value) {
    ASTNode* node = createNode(NODE_ASSIGN);
    node->data.assign.variable = variable;
    node->data.assign.value = value;
    return node;
}

// Create an operator node (add, sub, etc)
ASTNode* createOperatorNode(NodeType op_type, ASTNode* left, ASTNode* right) {
    ASTNode* node = createNode(op_type);
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

// Create a print node
ASTNode* createPrintNode(char* format, ASTNode* value) {
    ASTNode* node = createNode(NODE_PRINT);
    node->data.print.format = strdup(format);
    node->data.print.value = value;
    return node;
}

// Create an if node
ASTNode* createIfNode(ASTNode* condition, ASTNode* if_body, ASTNode* else_body) {
    ASTNode* node = createNode(NODE_IF);
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.if_body = if_body;
    node->data.if_stmt.else_body = else_body;
    return node;
}

// Create a for loop node
ASTNode* createForNode(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body) {
    ASTNode* node = createNode(NODE_FOR);
    node->data.for_stmt.init = init;
    node->data.for_stmt.condition = condition;
    node->data.for_stmt.increment = increment;
    node->data.for_stmt.body = body;
    return node;
}

// Free memory allocated for AST
void freeAST(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            for (int i = 0; i < node->data.block.count; i++) {
                freeAST(node->data.block.statements[i]);
            }
            free(node->data.block.statements);
            break;
            
        case NODE_VAR_DECL:
            free(node->data.decl.name);
            free(node->data.decl.type_name);
            break;
            
        case NODE_VAR:
            free(node->data.var.name);
            break;
            
        case NODE_ASSIGN:
            freeAST(node->data.assign.variable);
            freeAST(node->data.assign.value);
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
            if (node->data.for_stmt.init) {
                freeAST(node->data.for_stmt.init);
            }
            if (node->data.for_stmt.condition) {
                freeAST(node->data.for_stmt.condition);
            }
            if (node->data.for_stmt.increment) {
                freeAST(node->data.for_stmt.increment);
            }
            freeAST(node->data.for_stmt.body);
            break;
            
        default:
            break;
    }
    
    free(node);
}

void printASTWithIndent(ASTNode* node, int indent) {
    if (!node) return;
    
    // Add indentation
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            
        printf("(\n");
            if (node->data.block.count > 0) {
                // Print first statement
                printASTWithIndent(node->data.block.statements[0], indent + 1);
                
                // For each subsequent statement, open a new block with increasing indentation
                for (int i = 1; i < node->data.block.count; i++) {
                    printf("\n");
                    for (int j = 0; j < indent + 1; j++) {
                        printf("  ");
                    }
                    printf("(\n");
                    printASTWithIndent(node->data.block.statements[i], indent + 2);
                }
                
                // Close all the nested blocks with proper staircase indentation
                for (int i = node->data.block.count - 1; i >= 0; i--) {
                    printf("\n");
                    for (int j = 0; j < indent + 1 - (node->data.block.count - 1 - i); j++) {
                        printf("  ");
                    }
                    printf(")");
                }
                return; // Skip the default closing parenthesis
            }
            
            // For empty blocks
            printf("\n");
            for (int i = 0; i < indent; i++) {
                printf("  ");
            }
            printf(")");
            break;
            
        case NODE_VAR_DECL:
            printf("(%s %s)", node->data.decl.name, node->data.decl.type_name);
            break;
            
        case NODE_VAR:
            printf("%s", node->data.var.name);
            break;
            
        case NODE_NUMBER:
            printf("(%d %d)", node->data.num.value, node->data.num.base);
            break;
            
        case NODE_ADD:
            printf("(+ ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_SUB:
            printf("(- ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_MUL:
            printf("(* ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_DIV:
            printf("(/ ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_MOD:
            printf("(%% ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_GREATER:
            printf("(> ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_LESS:
            printf("(< ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_EQUAL:
            printf("(== ");
            printASTWithIndent(node->data.op.left, 0);
            printf(" ");
            printASTWithIndent(node->data.op.right, 0);
            printf(")");
            break;
            
        case NODE_ASSIGN:
            printf("(:= ");
            printASTWithIndent(node->data.assign.variable, 0);
            printf(" ");
            printASTWithIndent(node->data.assign.value, 0);
            printf(")");
            break;
            
        case NODE_PRINT:
            printf("(print \"%s\" ", node->data.print.format);
            printASTWithIndent(node->data.print.value, 0);
            printf(")");
            break;
            
        case NODE_IF:
            printf("(if \n");
            printASTWithIndent(node->data.if_stmt.condition, 0);
            printf(" ");
            printASTWithIndent(node->data.if_stmt.if_body, 0);
            if (node->data.if_stmt.else_body) {
                printf(" ");
                printASTWithIndent(node->data.if_stmt.else_body, 0);
            }
            printf(")");
            break;
            
        case NODE_FOR:
            printf("(for ");
            if (node->data.for_stmt.init) {
                printASTWithIndent(node->data.for_stmt.init, 0);
                printf(" ");
            } else {
                printf("() ");
            }
            
            printASTWithIndent(node->data.for_stmt.condition, 0);
            printf(" ");
            
            if (node->data.for_stmt.increment) {
                printASTWithIndent(node->data.for_stmt.increment, 0);
                printf(" ");
            } else {
                printf("() ");
            }
            
            printASTWithIndent(node->data.for_stmt.body, 0);
            printf(")");
            break;
            
        default:
            printf("(UNKNOWN_NODE)");
            break;
    }
}

// Wrapper function for the indented print function
void printAST(ASTNode* node) {
    printASTWithIndent(node, 0);
}