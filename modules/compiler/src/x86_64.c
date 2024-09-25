#include "uniform/compiler/shared.h"

// ============================
//          Forwards
// ============================

static void set_iopt_flag(UniformCompiler*, UNIFORM_IOPT_X86_64_FLAG);
static void clear_iopt_flag(UniformCompiler*, UNIFORM_IOPT_X86_64_FLAG);
static int iopt_flag_set(UniformCompiler*, UNIFORM_IOPT_X86_64_FLAG);

static int compile(UniformCompiler* compiler, UniformAST* tree);
static int compile_unknown_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static int compile_literal_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static int compile_operator_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static int compile_expression_node(UniformCompiler*, UniformAST*, UniformASTNode*, int);
static UniformASTNode* peek_node(UniformAST*, int, int);

// ============================
//        Implementation
// ============================

static int iopt_flag_set(UniformCompiler* compiler, UNIFORM_IOPT_X86_64_FLAG flag) {
  return (compiler->iopt_flags & (int)flag) == (int)flag;
}

static void set_iopt_flag(UniformCompiler* compiler, UNIFORM_IOPT_X86_64_FLAG flag) {
  if (iopt_flag_set(compiler, flag)) {
    printf("\n\n fatal: flag set and was never cleared (%d) \n\n", flag);
  }

  compiler->iopt_flags |= (int)flag;
}

static void clear_iopt_flag(UniformCompiler* compiler, UNIFORM_IOPT_X86_64_FLAG flag) {
  compiler->iopt_flags &= ~(int)flag;
}

static UniformASTNode* peek_node(UniformAST* tree, int index, int n) {
  if (index + n >= tree->used) {
    return NULL;
  }

  return tree->nodes[index + n];
}

// ------------------------------------

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

  // simple expressions, 2 + 2
  if (next_node != NULL && next_node->type == UNIFORM_LITERAL_NODE) {
    set_iopt_flag(compiler, UNIFORM_IOPT_RAX_SET);
    printf("\t mov rax, %d\n", data->literal.value.i32);
    return 0;
  }

  // second node of simple expressions, 2 + 2
  if (iopt_flag_set(compiler, UNIFORM_IOPT_RAX_SET)) {
    set_iopt_flag(compiler, UNIFORM_IOPT_RDX_SET);
    printf("\t mov rdx, %d\n", data->literal.value.i32);
    return 0;
  }

  // unary operations
  if (next_node != NULL && next_node->type == UNIFORM_OPERATOR_NODE) {
    UniformASTOperatorNode* opnode = (UniformASTOperatorNode*)next_node->data;
    if (opnode->operator == T_NEGATE) {
      set_iopt_flag(compiler, UNIFORM_IOPT_RAX_SET);
      printf("\t mov rax, %d\n", data->literal.value.i32);
      return 0;
    }
  }

  printf("\t push %d\n", data->literal.value.i32);

  return 0;
}

static int compile_operator_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node, int index) {
  UniformASTOperatorNode* data = (UniformASTOperatorNode*)node->data;

  switch(data->operator) {
    case T_PLUS: {
      if (!iopt_flag_set(compiler, UNIFORM_IOPT_RAX_SET) && !iopt_flag_set(compiler, UNIFORM_IOPT_RDX_SET)) {
        printf("\n\t pop rdx\n");
        printf("\t pop rax\n");
      }
      printf("\t add rax, rdx\n");
      printf("\t push rax\n\n");
      break;
    }
    case T_MINUS: {
      if (!iopt_flag_set(compiler, UNIFORM_IOPT_RAX_SET) && !iopt_flag_set(compiler, UNIFORM_IOPT_RDX_SET)) {
        printf("\n\t pop rdx\n");
        printf("\t pop rax\n");
      }
      printf("\n\t sub rax, rdx\n");
      printf("\t push rax\n\n");
      break;
    }
    case T_STAR: {
      if (!iopt_flag_set(compiler, UNIFORM_IOPT_RAX_SET) && !iopt_flag_set(compiler, UNIFORM_IOPT_RDX_SET)) {
        printf("\n\t pop rdx\n");
        printf("\t pop rax\n");
      }
      printf("\t mul rdx\n");
      printf("\t push rax\n\n");
      break;
    }
    case T_SLASH: {
      if (!iopt_flag_set(compiler, UNIFORM_IOPT_RAX_SET) && !iopt_flag_set(compiler, UNIFORM_IOPT_RDX_SET)) {
        printf("\t pop rcx\n");
        printf("\t pop rax\n");
      } else {
        printf("\t xchg rcx, rdx\n");
      }
      printf("\n\t xor rdx, rdx \t ; SIGFPE unless 0 \n");
      printf("\t div rcx\n");
      printf("\t ; todo. rdx has the remainder. update to a internal float type\n");
      printf("\t push rax\n\n");
      break;
    }
    case T_NEGATE: {
      if (!iopt_flag_set(compiler, UNIFORM_IOPT_RAX_SET)) {
        printf("\t pop rax\n");
      }
      printf("\t neg rax\n");
      printf("\t push rax\n\n");
      break;
    }
    default:
      return 1;
  }

  clear_iopt_flag(compiler, UNIFORM_IOPT_RAX_SET);
  clear_iopt_flag(compiler, UNIFORM_IOPT_RDX_SET);

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
  printf("17 neg 49 + 4 / 2 3 * - 9 3 - 2 + * \n");
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
