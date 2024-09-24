#include "uniform/ast/shared.h"

// ============================
//          Forwards
// ============================

static struct UniformASTStruct* init_tree(int n_nodes);
static void insert_node(struct UniformASTStruct* tree, UniformASTNode* node);
static void free_tree(struct UniformASTStruct *tree);

// ============================
//        Implementation
// ============================

static struct UniformASTStruct* init_tree(int n_nodes) {
  struct UniformASTStruct *tree = malloc(sizeof(struct UniformASTStruct));
  tree->size = n_nodes;
  tree->used = 0;
  tree->nodes = malloc(sizeof(UniformASTNode*) * n_nodes);

  return tree;
}

static void insert_node(struct UniformASTStruct *tree, UniformASTNode *node) {
  if (tree->used == tree->size) {
    tree->size = (tree->size * 3) / 2 + 8;
    tree->nodes = (UniformASTNode**)(realloc(
      tree->nodes, tree->size * sizeof(UniformASTNode*)
    ));
  }

  tree->nodes[tree->used++] = node;
}

static void free_tree(struct UniformASTStruct *tree) {
  for (int i = 0; i < tree->used; i++) {
    // todo: cast to type based on node
    // free(tree->nodes[i]->data);
    free(tree->nodes[i]);
  }
  free(tree);
}

// ============================
//            Module
// ============================

struct UniformASTModuleStruct UniformASTModule = {
  .init_tree = init_tree,
  .insert_node = insert_node,
  .free_tree = free_tree
};