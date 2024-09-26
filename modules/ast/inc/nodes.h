#ifndef __UNIFORM_ASTNODESH
#define __UNIFORM_ASTNODESH 1

#include <string.h>

typedef struct UniformASTStruct UniformAST, UniformASTExpressionNode;

typedef enum {
  UNIFORM_UNKNOWN_NODE,
  UNIFORM_LITERAL_NODE,
  UNIFORM_OPERATOR_NODE,
  UNIFORM_EXPRESSION_NODE,
  UNIFORM_ASSIGNMENT_NODE
} UNIFORM_NODE_TYPE;

typedef struct UniformASTNodeStruct {
  UNIFORM_NODE_TYPE type;
  void *data;
} UniformASTNode;

typedef struct UniformASTLiteralNodeStruct {
  UniformScannerLiteral literal;
} UniformASTLiteralNode;

typedef struct UniformASTOperatorNodeStruct {
  UNIFORM_TOKEN_CODE operator;
} UniformASTOperatorNode;

typedef struct UniformASTAssignmentNodeStruct {
  char identifier[128];
  char data_type[128]; // todo, set to id pointer in symbol table
  UniformASTExpressionNode* expressions;
} UniformASTAssignmentNode;

struct UniformASTNodeModuleStruct {
  UniformASTNode* (*token_to_node)(UniformToken*);
};

extern struct UniformASTNodeModuleStruct UniformASTNodeModule;

#endif