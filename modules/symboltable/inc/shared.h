#ifndef __UNIFORM_SYMBOLTABLEH
#define __UNIFORM_SYMBOLTABLEH 1

#include <stdlib.h>
#include <string.h>

#include "uniform/tokens/shared.h"

struct UniformSymbolTableNodeStruct;
struct UniformSymbolTableStruct;

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
  UNIFORM_TYPE_DEFINITION
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
      int param_count;
      int param_size;
      int locals_size;

      struct UniformSymbolTableNodeStruct* params;
      struct UniformSymbolTableNodeStruct* locals;
      struct UniformSymbolTableStruct*     local_symbol_table;
    } func;
  } description;

  struct { int offset; } data;
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
  void (*clear)(UniformSymbolTable* table);
};

extern struct UniformSymbolTableModuleStruct UniformSymbolTableModule;

#endif