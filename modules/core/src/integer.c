#include "uniform/core/shared.h"

// ============================
//          Forwards
// ============================

static void init(UniformSymbolTable* table);

// ============================
//        Implementation
// ============================

static void init(UniformSymbolTable* table) {
  UniformSymbolTableNode* IntegerTypeNode = UniformSymbolTableModule.insert(table, "Integer");
  IntegerTypeNode->definition.type = UNIFORM_TYPE_DEFINITION;

  IntegerTypeNode->type = malloc(sizeof(UniformSymbolTableType));
  IntegerTypeNode->type->size = sizeof(int);
  IntegerTypeNode->type->form = UNIFORM_SCALAR_FORM;
  IntegerTypeNode->type->type_idp = IntegerTypeNode;
}

// ============================
//        Module
// ============================

struct UniformCoreIntegerModuleStruct UniformCoreIntegerModule = {
  .init = init
};