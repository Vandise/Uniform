#ifndef __UNIFORM_ASTH
#define __UNIFORM_ASTH 1

#include "uniform/symboltable/shared.h"

typedef struct UniformASTStruct {
  int size;
  int used;

  UniformSymbolTableType *type;

  UniformASTNode** nodes;
} UniformAST, UniformASTExpressionNode, UniformASTBodyNode, UniformASTBlockNode;

struct UniformASTModuleStruct {
  struct UniformASTStruct* (*init_tree)(int);
  void (*insert_node)(struct UniformASTStruct*, UniformASTNode*);
  void (*free_tree)(struct UniformASTStruct*);
  UniformASTNode* (*token_to_node)(UniformToken*);
};

extern struct UniformASTModuleStruct UniformASTModule;

#endif