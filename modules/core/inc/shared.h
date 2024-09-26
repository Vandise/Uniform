#ifndef __UNIFORM_CORELIBH
#define __UNIFORM_CORELIBH 1

#include "uniform/symboltable/shared.h"

struct UniformCoreIntegerModuleStruct {
  void (*init)(UniformSymbolTable*);
} UniformCoreIntegerModule;

extern struct UniformCoreIntegerModuleStruct UniformCoreIntegerModule;

#endif