#include "uniform/compiler/shared.h"

// ============================
//          Forwards
// ============================

static void set_iopt_flag(UniformCompiler*, UNIFORM_IOPT_X86_64_FLAG);
static void clear_iopt_flag(UniformCompiler*, UNIFORM_IOPT_X86_64_FLAG);
static int iopt_flag_set(UniformCompiler*, UNIFORM_IOPT_X86_64_FLAG);

static int compile(UniformCompiler* compiler, UniformAST* tree);
static int compile_unknown_node(UniformCompiler*, UniformAST*, UniformASTNode*);
static int compile_literal_node(UniformCompiler*, UniformAST*, UniformASTNode*);
static int compile_operator_node(UniformCompiler*, UniformAST*, UniformASTNode*);
static int compile_expression_node(UniformCompiler*, UniformAST*, UniformASTNode*);
static UniformASTNode* peek_node(UniformAST*, int, int);

// ============================
//        Implementation
// ============================

static int iopt_flag_set(UniformCompiler* compiler, UNIFORM_IOPT_X86_64_FLAG flag) {
  return ((compiler->iopt_flags ) & (1<<(flag)));
}

static void set_iopt_flag(UniformCompiler* compiler, UNIFORM_IOPT_X86_64_FLAG flag) {
  ((compiler->iopt_flags) |=  (1<<(flag)));
}

static void clear_iopt_flag(UniformCompiler* compiler, UNIFORM_IOPT_X86_64_FLAG flag) {
  ((compiler->iopt_flags) &= ~(1<<(flag)));
}

static UniformASTNode* peek_node(UniformAST* tree, int index, int n) {
  if (index + (n) >= tree->used) {
    return NULL;
  }
  return tree->nodes[index + (n)];
}

// ------------------------------------

static int compile_unknown_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node) {
  printf("Error\n");
  return 1;
}

//
// todo:
//  case 1 should set rcx for div instruction
//  case 2 literal should be arg for instruction as opposed to set to reg rcx/rdx
//
static int compile_literal_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node) {
  UniformASTLiteralNode* data = (UniformASTLiteralNode*)node->data;

  UniformASTNode* insp_node;
  int istr_cnt  = 0;

  if (src_tree->used > 1) {
    do {
      insp_node = peek_node(src_tree, compiler->node_index, istr_cnt + 1);
      istr_cnt++;

      // unary
      if (insp_node != NULL && insp_node->type == UNIFORM_OPERATOR_NODE) {
        UniformASTOperatorNode* opnode = (UniformASTOperatorNode*)(insp_node->data);
        if (opnode->operator == T_NEGATE) {
          istr_cnt++;
        }
      }
    } while (insp_node != NULL && insp_node->type != UNIFORM_OPERATOR_NODE);
  }

  switch(istr_cnt) {
    // single instruction
    case 0: {
      printf("\t mov rax, %d\n", data->literal.value.i32);
      break;
    }
    // second arg
    case 1: {
      printf("\t mov rdx, %d\n", data->literal.value.i32);
      set_iopt_flag(compiler, UIOPT_RDX_SET);
      break;
    }
    // first arg
    case 2: {
      if (iopt_flag_set(compiler,UIOPT_RAX_SET)) {
        printf("\t push rax\n\n");
      }

      printf("\t mov rax, %d\n", data->literal.value.i32);
      set_iopt_flag(compiler, UIOPT_RAX_SET);
      break;
    }
    default: {
      printf("\t push %d\n", data->literal.value.i32);
    }
  }

  return 0;
}

static int compile_operator_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node) {
  UniformASTOperatorNode* data = (UniformASTOperatorNode*)node->data;

  int pnode_was_op = 0;
  UniformASTNode* previous_node = peek_node(src_tree, compiler->node_index, -1);
  if (previous_node != NULL && previous_node->type == UNIFORM_OPERATOR_NODE) {
    pnode_was_op = 1;
  }

  //
  // todo: duplicate logic
  //
  switch(data->operator) {

    case T_PLUS: {
      if (!iopt_flag_set(compiler,UIOPT_RDX_SET)) {
        printf("\t pop rdx\n");
      } else {
        clear_iopt_flag(compiler,UIOPT_RDX_SET);
      }

      if (!iopt_flag_set(compiler,UIOPT_RAX_SET)) {
        printf("\t pop rax\n");
      } else {
        clear_iopt_flag(compiler,UIOPT_RAX_SET);
      }
      printf("\t add rax, rdx\n");
      set_iopt_flag(compiler, UIOPT_RAX_SET);
      break;
    }

    case T_MINUS: {
      if (pnode_was_op && iopt_flag_set(compiler, UIOPT_RAX_SET)) {
        printf("\n\t mov rdx, rax\n");
      }

      if (!iopt_flag_set(compiler,UIOPT_RDX_SET)) {
        if (pnode_was_op) {
          printf("\t pop rax\n");
        } else {
          printf("\t pop rdx\n");
        }
      } else {
        clear_iopt_flag(compiler,UIOPT_RDX_SET);
      }

      if (!iopt_flag_set(compiler,UIOPT_RAX_SET)) {
        printf("\t pop rax\n");
      } else {
        clear_iopt_flag(compiler,UIOPT_RAX_SET);
      }

      printf("\n\t sub rax, rdx\n");

      set_iopt_flag(compiler, UIOPT_RAX_SET);
      break;
    }

    case T_STAR: {
      if (!iopt_flag_set(compiler, UIOPT_RDX_SET)) {
        printf("\t pop rdx\n");
      } else {
        clear_iopt_flag(compiler, UIOPT_RDX_SET);
      }

      if (!iopt_flag_set(compiler, UIOPT_RAX_SET)) {
        printf("\t pop rax\n");
      } else {
        clear_iopt_flag(compiler,UIOPT_RAX_SET);
      }

      printf("\t mul rdx\n");

      set_iopt_flag(compiler, UIOPT_RAX_SET);
      break;
    }

    case T_SLASH: {
      if (pnode_was_op && iopt_flag_set(compiler, UIOPT_RAX_SET)) {
        printf("\n\t mov rcx, rax\n");
      }

      if (!iopt_flag_set(compiler,UIOPT_RDX_SET)) {
        if (pnode_was_op) {
          printf("\t pop rax\n");
        } else {
          printf("\t pop rcx\n");
        }
      } else {
        if (!pnode_was_op) { printf("\t xchg rcx, rdx\n"); }
        clear_iopt_flag(compiler,UIOPT_RDX_SET);
      }

      if (!iopt_flag_set(compiler,UIOPT_RAX_SET)) {
        printf("\t pop rax\n");
      } else {
        clear_iopt_flag(compiler,UIOPT_RAX_SET);
      }

      printf("\t xor rdx, rdx \t ; SIGFPE unless 0 \n");
      printf("\t div rcx\n");
      printf("\t ; todo. rdx has the remainder. update to a internal float type\n");

      set_iopt_flag(compiler, UIOPT_RAX_SET);
      break;
    }

    case T_NEGATE: {
      if (!iopt_flag_set(compiler,UIOPT_RAX_SET)) {
        printf("\t pop rax\n");
        printf("\t neg rax\n");
      } else {
        printf("\t neg rax\n");
      }
      set_iopt_flag(compiler, UIOPT_RAX_SET);
      break;
    }
    default:
      return 1;
  }

  return 0;
}

static int compile_expression_node(UniformCompiler* compiler, UniformAST* src_tree, UniformASTNode* node) {
  UniformAST* data = (UniformAST*)node->data;
  return 0;
}

// actions matching UNIFORM_NODE_TYPE
static int (*compile_actions[])(UniformCompiler*, UniformAST*, UniformASTNode*) = {
  compile_unknown_node,
  compile_literal_node,
  compile_operator_node,
  compile_expression_node
};

static int compile(UniformCompiler* compiler, UniformAST* tree) {
  int result = 0;
  for (compiler->node_index = 0; compiler->node_index < tree->used; compiler->node_index++) {
    result = compile_actions[tree->nodes[compiler->node_index]->type](compiler, tree, tree->nodes[compiler->node_index]);
  }
  return result;
}

// ============================
//            Module
// ============================

struct UniformCompilerX8664ModuleStruct UniformCompilerX86_64Module = {
  .compile = compile
};
