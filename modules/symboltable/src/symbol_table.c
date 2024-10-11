#include "uniform/symboltable/shared.h"

// ============================
//          Forwards
// ============================
static UniformSymbolTableNode* search_tree(UniformSymbolTableNode* node, char *name);
static UniformSymbolTableNode* insert_node(UniformSymbolTableNode** location_node, UniformSymbolTableNode* node);

static UniformSymbolTableNode* symboltable_search_global(UniformSymbolTable* table, char *name);
static UniformSymbolTableNode* symboltable_insert_global(UniformSymbolTable* table, char *name);
static UniformSymbolTableNode* symboltable_insert_module(UniformSymbolTable* table, UniformSymbolTableNode* node);

static void clear(UniformSymbolTable* table);
static void print_tree(UniformSymbolTableNode *root, int level);

// ============================
//        Implementation
// ============================

static UniformSymbolTable* init() {
  UniformSymbolTable* table = malloc(sizeof(UniformSymbolTable));
  table->global_node = NULL;
  table->current_scope = 0;

  return table;
}

static UniformSymbolTableNode* search_tree(UniformSymbolTableNode* node, char *name) {
  while(node != NULL) {
    int str_cmp = strcmp(name, node->name);
    if (str_cmp == 0) {
      return node;
    }
    node = str_cmp < 0 ? node->left : node->right; 
  }
  return NULL;
}

static UniformSymbolTableNode* insert_node(UniformSymbolTableNode** location_node, UniformSymbolTableNode* node) {
  UniformSymbolTableNode*  test_node = NULL;
  node->left = node->right = node->next = NULL;

  int str_cmp = 0;
  while((test_node = *location_node) != NULL) {
    str_cmp = strcmp(node->name, test_node->name);
    location_node = str_cmp < 0 ? &(test_node->left) : &(test_node->right);
  }

  *location_node = node;

  return node;
}

static UniformSymbolTableNode* symboltable_search_global(UniformSymbolTable* table, char *name) {
  return search_tree(table->global_node, name);
}

static UniformSymbolTableNode* symboltable_insert_global(UniformSymbolTable* table, char *name) {
  UniformSymbolTableNode*  node = malloc(sizeof(UniformSymbolTableNode));
  node->name = malloc(strlen(name) + 1);
  strcpy(node->name, name);
  node->definition.type = UNIFORM_UNDEFINED_DEFINITION;

  // needs ** as inserting a node causes a pointer to be changed
  return insert_node(&(table->global_node), node);
}

static UniformSymbolTableNode* symboltable_insert_module(UniformSymbolTable* table, UniformSymbolTableNode* node) {
  UniformSymbolTableNode** location_node;
  UniformSymbolTableNode*  parent = node->definition.info.module.parent;

  node->definition.type = UNIFORM_MODULE_DEFINITION;
  node->definition.info.module.symbol_table = init();

  if (parent != NULL) {
    location_node = &(parent->definition.info.module.symbol_table->global_node);
  } else {
    location_node = &(table->global_node);
  }

  return insert_node(location_node, node);
}

static void clear_nodes(UniformSymbolTableNode* node) {
  if (node == NULL) {
    return;
  }

  clear_nodes(node->next);
  clear_nodes(node->right);
  clear_nodes(node->left);

  free(node->name);

  // type pointers can only clear themselves
  if (node->type != NULL && node->definition.type == UNIFORM_TYPE_DEFINITION) {
    free(node->type);
    node->type = NULL;
  }

  // modules
  if (node->definition.type == UNIFORM_MODULE_DEFINITION) {
    clear(node->definition.info.module.symbol_table);
  }

  // functions
  if (node->definition.type == UNIFORM_FUNCTION_DEFINITION) {
    clear(node->definition.info.func.local_symbol_table);
  }

  free(node);
}

static void clear(UniformSymbolTable* table) {
  clear_nodes(table->global_node);
  free(table);
}

static void print_tree(UniformSymbolTableNode *root, int level) {
  if (root == NULL) {
    return;
  }

  for (int i = 0; i < level; i++) {
    printf(i == level - 1 ? "|-" : "  ");
  }
  
  printf("%s\n", root->name);

  print_tree(root->left, level + 1);
  print_tree(root->right, level + 1);
}

// ============================
//        Module
// ============================

struct UniformSymbolTableModuleStruct UniformSymbolTableModule = {
  .init   = init,
  .search_global = symboltable_search_global,
  .insert_global = symboltable_insert_global,
  .insert_module = symboltable_insert_module,
  .clear =  clear,
  .print_tree = print_tree
};