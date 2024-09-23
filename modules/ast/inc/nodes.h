#ifndef __UNIFORM_ASTNODESH
#define __UNIFORM_ASTNODESH 1

typedef enum {
  UNIFORM_UNKNOWN_NODE,
  UNIFORM_LITERAL_NODE,
  UNIFORM_EXPRESSION_NODE
} UNIFORM_NODE_TYPE;

typedef struct UniformASTNodeStruct {
  UNIFORM_NODE_TYPE type;
  void *data;
} UniformASTNode;

typedef struct UniformASTOperatorNodeStruct {
  UNIFORM_TOKEN_CODE operator;
} UniformASTOperator;

struct UniformASTNodeModuleStruct {
  UniformASTNode* (*token_to_node)(UniformToken*);
};

extern struct UniformASTNodeModuleStruct UniformASTNodeModule;

#endif