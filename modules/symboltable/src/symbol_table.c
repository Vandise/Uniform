#include "uniform/symboltable/shared.h"

// ============================
//          Forwards
// ============================

static UniformSymbolTableNode* symboltable_search(UniformSymbolTable* table, char *name);
static UniformSymbolTableNode* symboltable_insert(UniformSymbolTable* table, char *name);

// ============================
//        Implementation
// ============================

static UniformSymbolTable* init() {
  UniformSymbolTable* table = malloc(sizeof(UniformSymbolTable));
  table->root_node = NULL;

  return table;
}

static UniformSymbolTableNode* symboltable_search(UniformSymbolTable* table, char *name) {
  UniformSymbolTableNode* node = table->root_node;
  while(node != NULL) {
    int str_cmp = strcmp(name, node->name);
    if (str_cmp == 0) {
      return node;
    }
    node = str_cmp < 0 ? node->left : node->right; 
  }
  return NULL;
}

static UniformSymbolTableNode* symboltable_insert(UniformSymbolTable* table, char *name) {
  UniformSymbolTableNode** location_node = &(table->root_node); // inserting a node causes a pointer to be changed
  UniformSymbolTableNode*  test_node;
  UniformSymbolTableNode*  node = malloc(sizeof(UniformSymbolTableNode));

  node->name = malloc(strlen(name) + 1);
  strcpy(node->name, name);

  node->left = node->right = node->next = NULL;

  int str_cmp = 0;
  while((test_node = *location_node) != NULL) {
    str_cmp = strcmp(name, test_node->name);
    location_node = str_cmp < 0 ? &(test_node->left) : &(test_node->right);
  }

  *location_node = node;

  return node;
}

static void clear_nodes(UniformSymbolTableNode* node) {
  if (node == NULL) {
    return;
  }

  clear_nodes(node->next);
  clear_nodes(node->right);
  clear_nodes(node->left);

  free(node->name);
  free(node);
}

static void clear(UniformSymbolTable* table) {
  clear_nodes(table->root_node);
  free(table);
}

// ============================
//        Module
// ============================

struct UniformSymbolTableModuleStruct UniformSymbolTableModule = {
  .init   = init,
  .search = symboltable_search,
  .insert = symboltable_insert,
  .clear =  clear
};