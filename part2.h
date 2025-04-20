#ifndef PART2_H
#define PART2_H

// Node types for the AST
typedef enum {
    NODE_PROGRAM,    // Program root
    NODE_BLOCK,      // Block of statements
    NODE_VAR_DECL,   // Variable declaration
    NODE_VAR,        // Variable reference
    NODE_NUMBER,     // Number literal
    NODE_ADD,        // Addition
    NODE_SUB,        // Subtraction
    NODE_MUL,        // Multiplication
    NODE_DIV,        // Division
    NODE_MOD,        // Modulo
    NODE_GREATER,    // Greater than
    NODE_LESS,       // Less than
    NODE_EQUAL,      // Equal to
    NODE_ASSIGN,     // Assignment
    NODE_PRINT,      // Print statement
    NODE_IF,         // If statement
    NODE_FOR         // For loop
} NodeType;

// Forward declaration of the ASTNode struct
typedef struct ASTNode ASTNode;

// Node structure
struct ASTNode {
    NodeType type;
    
    // Data specific to the node type
    union {
        // For block-like nodes (program, block)
        struct {
            int count;
            int capacity;
            ASTNode** statements;
        } block;
        
        // For variable declarations
        struct {
            char* name;
            char* type_name;
        } decl;
        
        // For variable references
        struct {
            char* name;
        } var;
        
        // For number literals
        struct {
            int value;
            int base;
        } num;
        
        // For binary operators
        struct {
            ASTNode* left;
            ASTNode* right;
        } op;
        
        // For assignments
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
            ASTNode* increment;
            ASTNode* body;
        } for_stmt;
    } data;
    
    // Next statement in a list
    ASTNode* next;
};

// AST functions
ASTNode* createNode(NodeType type);
ASTNode* createProgramNode();
ASTNode* createBlockNode();
void addStatementToBlock(ASTNode* block, ASTNode* statement);
ASTNode* createVarDeclNode(char* name, char* type_name);
ASTNode* createVariableNode(char* name);
ASTNode* createNumberNode(int value, int base);
ASTNode* createAssignNode(ASTNode* variable, ASTNode* value);
ASTNode* createOperatorNode(NodeType op_type, ASTNode* left, ASTNode* right);
ASTNode* createPrintNode(char* format, ASTNode* value);
ASTNode* createIfNode(ASTNode* condition, ASTNode* if_body, ASTNode* else_body);
ASTNode* createForNode(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body);
void freeAST(ASTNode* node);
void printAST(ASTNode* node);

#endif /* PART2_H */