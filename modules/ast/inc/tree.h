#ifndef __UNIFORM_ASTH
#define __UNIFORM_ASTH 1

typedef struct UniformASTStruct {
  int size;
  int used;

  UniformASTNode** nodes;
} UniformAST, UniformASTExpressionNode;

struct UniformASTModuleStruct {
  struct UniformASTStruct* (*init_tree)(int);
  void (*insert_node)(struct UniformASTStruct*, UniformASTNode*);
  void (*free_tree)(struct UniformASTStruct*);
  UniformASTNode* (*token_to_node)(UniformToken*);
};

extern struct UniformASTModuleStruct UniformASTModule;

#endif