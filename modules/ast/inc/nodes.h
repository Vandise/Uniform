#ifndef __UNIFORM_ASTNODESH
#define __UNIFORM_ASTNODESH 1

#include <string.h>
#include "uniform/symboltable/shared.h"

typedef struct UniformASTStruct UniformAST, UniformASTExpressionNode, UniformASTBodyNode, UniformASTBlockNode;

typedef enum {
  UNIFORM_UNKNOWN_NODE,
  UNIFORM_LITERAL_NODE,
  UNIFORM_OPERATOR_NODE,
  UNIFORM_EXPRESSION_NODE,
  UNIFORM_ASSIGNMENT_NODE,
  UNIFORM_MODULE_NODE,
  UNIFORM_CONSTANT_NODE,
  UNIFORM_FUNCTION_DECLARATION_NODE
} UNIFORM_NODE_TYPE;

typedef struct UniformASTNodeStruct {
  UNIFORM_NODE_TYPE type;
  void *data;
} UniformASTNode;

typedef struct UniformASTModuleNodeStruct {
  struct UniformASTModuleNodeStruct* parent;

  char identifier[128];
  UniformSymbolTableNode* symbol;
  UniformASTBodyNode* body;
} UniformASTModuleNode;

typedef struct UniformASTFunctionDeclarationNodeStruct {
  struct UniformASTFunctionDeclarationNodeStruct* parent;
  
  int private_flag;
  UniformSymbolTableNode* return_type;

  char identifier[128];
  UniformSymbolTableNode* symbol;
  UniformASTBodyNode* body;
  UniformASTModuleNode* module;
} UniformASTFunctionDeclarationNode, UniformASTLambdaDeclarationNode;

typedef struct UniformASTLiteralNodeStruct {
  UniformScannerLiteral literal;
} UniformASTLiteralNode;

typedef struct UniformASTOperatorNodeStruct {
  UNIFORM_TOKEN_CODE operator;
} UniformASTOperatorNode;

typedef struct UniformASTAssignmentNodeStruct {
  char identifier[128];

  UniformSymbolTableNode* type;
  UniformSymbolTableNode* symbol;

  UniformASTModuleNode* module;
  UniformASTExpressionNode* expressions;
} UniformASTAssignmentNode, UniformASTConstantNode;

struct UniformASTNodeModuleStruct {
  UniformASTNode* (*token_to_node)(UniformToken*);
};

extern struct UniformASTNodeModuleStruct UniformASTNodeModule;

#endif