#ifndef __UNIFORM_SYMBOLTABLEH
#define __UNIFORM_SYMBOLTABLEH 1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "uniform/tokens/shared.h"

struct UniformSymbolTableNodeStruct;
struct UniformSymbolTableStruct;
struct UniformSymbolTableTypeStruct;

typedef union {
  int i32;
  long int i64;
  float f32;
  double f64;
  char* string;
} UniformValue;

typedef enum {
  UNIFORM_UNDEFINED_DEFINITION,
  UNIFORM_CONSTANT_DEFINITION,
  UNIFORM_VARIABLE_DEFINITION,
  UNIFORM_FUNCTION_DEFINITION,
  UNIFORM_STRUCT_DEFINITION,
  UNIFORM_STRUCT_FIELD_DEFINITION,
  UNIFORM_TYPE_DEFINITION,
  UNIFORM_MODULE_DEFINITION
} UNIFORM_DEFINITION_TYPE;

typedef enum {
  UNIFORM_NO_FORM,
  UNIFORM_SCALAR_FORM
} UNIFORM_TYPE_FORM;

typedef struct UniformDefinitionStruct {
  UNIFORM_DEFINITION_TYPE type;

  union {
    struct {
      UniformValue value;
    } constant;

    struct {
      struct UniformSymbolTableNodeStruct* parent;
      struct UniformSymbolTableStruct* symbol_table;
    } module;

    struct {
      int param_count;
      int param_size;
      int locals_count;
      int locals_size;

      struct UniformSymbolTableTypeStruct* return_type;
      struct UniformSymbolTableNodeStruct* params;
      struct UniformSymbolTableNodeStruct* locals;
      struct UniformSymbolTableStruct*     local_symbol_table;
    } func;

    struct { int offset; } data;
  } info;
} UniformDefinition;

typedef struct UniformSymbolTableTypeStruct {
  UNIFORM_TYPE_FORM form;
  int size;
  struct UniformSymbolTableNodeStruct* type_idp;
} UniformSymbolTableType;

typedef struct UniformSymbolTableNodeStruct {
  struct UniformSymbolTableNodeStruct *left, *right;
  struct UniformSymbolTableNodeStruct *next;

  char *name;
  char *info;

  UniformDefinition definition;
  UniformSymbolTableType* type;

  char label[255];
  int level;
  int label_index;
} UniformSymbolTableNode;

typedef struct UniformSymbolTableStruct {
  UniformSymbolTableNode* global_node;

  int current_scope;
  UniformSymbolTableNode* local_scope[0x80];
} UniformSymbolTable;

struct UniformSymbolTableModuleStruct {
  UniformSymbolTable* (*init)();

  UniformSymbolTableNode* (*search_global)(UniformSymbolTable* table, char *name);
  UniformSymbolTableNode* (*insert_global)(UniformSymbolTable* table, char *name);
  UniformSymbolTableNode* (*insert_module)(UniformSymbolTable* table, UniformSymbolTableNode* node);

  void (*clear)(UniformSymbolTable* table);
  void (*print_tree)(UniformSymbolTableNode *root, int level);
};

extern struct UniformSymbolTableModuleStruct UniformSymbolTableModule;

#endif