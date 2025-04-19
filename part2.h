#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum {
    NODE_NUMBER,          // Number literal with base (10, 10)
    NODE_VARIABLE,        // Variable reference
    NODE_ADD,             // Binary operation +
    NODE_SUB,             // Binary operation -
    NODE_MUL,             // Binary operation *
    NODE_DIV,             // Binary operation /
    NODE_MOD,             // Binary operation %
    NODE_ASSIGN,          // Assignment :=
    NODE_VAR_DECL,        // Variable declaration
    NODE_PRINT,           // Print statement
    NODE_GREATER,         // Relational operator >
    NODE_LESS,            // Relational operator <
    NODE_EQUAL,           // Relational operator ==
    NODE_IF,              // If statement
    NODE_FOR,             // For loop
    NODE_WHILE,           // While loop
    NODE_STRING,          // String literal
    NODE_PROGRAM,         // Whole program
    NODE_BLOCK            // Block of statements
} NodeType;

// Forward declaration
typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    union {
        // For numbers like (10, 10)
        struct {
            int value;
            int base;
        } number;
        
        // For variables and strings
        char* name;
        
        // For binary operations and conditions
        struct {
            ASTNode* left;
            ASTNode* right;
        } op;
        
        // For variable declarations
        struct {
            char* name;
            char* type_name;
        } decl;
        
        // For assignment statements
        struct {
            ASTNode* variable;
            ASTNode* value;
        } assign;
        
        // For print statements
        struct {
            char* format;
            ASTNode* value;
        } print;
        
        // For if statements
        struct {
            ASTNode* condition;
            ASTNode* if_body;
            ASTNode* else_body;
        } if_stmt;
        
        // For for loops
        struct {
            ASTNode* init;
            ASTNode* condition;
            ASTNode* update;
            ASTNode* body;
        } for_loop;
        
        // For blocks of statements
        struct {
            ASTNode** statements;
            int count;
        } block;
    } data;
    
    ASTNode* next;  // For linked list of statements
};

// Create AST nodes
ASTNode* createNumberNode(int value, int base);
ASTNode* createVariableNode(char* name);
ASTNode* createStringNode(char* value);
ASTNode* createOperatorNode(NodeType type, ASTNode* left, ASTNode* right);
ASTNode* createAssignNode(ASTNode* variable, ASTNode* value);
ASTNode* createVarDeclNode(char* name, char* type);
ASTNode* createPrintNode(char* format, ASTNode* value);
ASTNode* createIfNode(ASTNode* condition, ASTNode* if_body, ASTNode* else_body);
ASTNode* createForNode(ASTNode* init, ASTNode* condition, ASTNode* update, ASTNode* body);
ASTNode* createBlockNode();
void addStatementToBlock(ASTNode* block, ASTNode* statement);
ASTNode* createProgramNode();

// Print and free AST
void printAST(ASTNode* node);
void freeAST(ASTNode* node);

#endif // AST_H