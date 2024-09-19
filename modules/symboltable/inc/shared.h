#ifndef __UNIFORM_SYMBOLTABLEH
#define __UNIFORM_SYMBOLTABLEH 1

#include <stdlib.h>
#include <string.h>

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
  UNIFORM_STRUCT_FIELD_DEFINITION
} UNIFORM_DEFINITION_TYPE;

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
  } definition;
} UniformDefinition;

typedef struct UniformSymbolTableNodeStruct {
  struct UniformSymbolTableNodeStruct *left, *right;
  struct UniformSymbolTableNodeStruct *next;

  char *name;
  char *info;

  int level;
  int label_index;

  UniformDefinition definition;
} UniformSymbolTableNode;

typedef struct UniformSymbolTableStruct {
  UniformSymbolTableNode* root_node;
} UniformSymbolTable;

struct UniformSymbolTableModuleStruct {
  UniformSymbolTable* (*init)();
  UniformSymbolTableNode* (*search)(UniformSymbolTable* table, char *name);
  UniformSymbolTableNode* (*insert)(UniformSymbolTable* table, char *name);
  void (*clear)(UniformSymbolTable* table);
};

extern struct UniformSymbolTableModuleStruct UniformSymbolTableModule;

#endif