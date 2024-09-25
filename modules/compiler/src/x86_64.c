#include "uniform/compiler/shared.h"

// ============================
//          Forwards
// ============================

static int compile(UniformCompiler* compiler, UniformAST* tree);
static int compile_unknown_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static int compile_literal_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static int compile_operator_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static int compile_expression_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static UniformASTNode* peek_node(UniformAST*, int, int);

// ============================
//        Implementation
// ============================

static UniformASTNode* peek_node(UniformAST* tree, int index, int n) {
  if (index + n >= tree->used) {
    return NULL;
  }

  return tree->nodes[index + n];
}

static int compile_unknown_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node, int index) {
  printf("Error\n");
  return 1;
}

//
// todo: unoptimized! use compiler struct for flags and peek_node +- n
//

static int compile_literal_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node, int index) {
  UniformASTLiteralNode* data = (UniformASTLiteralNode*)node->data;

  UniformASTNode* next_node = peek_node(src_tree, index, 1);

  printf("\t push %d\n", data->literal.value.i32);

  return 0;
}

static int compile_operator_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node, int index) {
  UniformASTOperatorNode* data = (UniformASTOperatorNode*)node->data;

  switch(data->operator) {
    case T_PLUS:
      printf("\n\t pop rdx\n");
      printf("\t pop rax\n");
      printf("\t add rax, rdx\n");
      printf("\t push rax\n\n");
      return 0;
    case T_MINUS:
      printf("\n\t pop rdx\n");
      printf("\t pop rax\n");
      printf("\n\t sub rax, rdx\n");
      printf("\t push rax\n\n");
      return 0;
    case T_STAR:
      printf("\n\t pop rdx\n");
      printf("\t pop rax\n");
      printf("\t mul rdx\n");
      printf("\t push rax\n\n");
      return 0;
    case T_SLASH:
      printf("\n\t xor rdx, rdx \t ; SIGFPE unless 0 \n");
      printf("\t pop rcx\n");
      printf("\t pop rax\n");
      //printf("\t xchg rax, rcx\n");
      printf("\t div rcx\n");
      printf("\t ; todo. rdx has the remainder. update to a internal float type\n");
      printf("\t push rax\n\n");
      return 0;
    case T_NEGATE:
      printf("\n\t pop rax\n");
      printf("\t neg rax\n");
      printf("\t push rax\n\n");
      return 0;
    default:
      return 1;
  }
  return 0;
}

static int compile_expression_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node, int index) {
  UniformAST* data = (UniformAST*)node->data;
  return 0;
}

// actions matching UNIFORM_NODE_TYPE
static int (*compile_actions[])(UniformCompiler*, UniformAST*, UniformASTNode*, int) = {
  compile_unknown_node,
  compile_literal_node,
  compile_operator_node,
  compile_expression_node
};

static int compile(UniformCompiler* compiler, UniformAST* tree) {
  int result = 0;
  for (int i = 0; i < tree->used; i++) {
    result = compile_actions[tree->nodes[i]->type](compiler, tree, tree->nodes[i], i);
  }

  return result;
}

// ============================
//            Module
// ============================

struct UniformCompilerX8664ModuleStruct UniformCompilerX86_64Module = {
  .compile = compile
};
