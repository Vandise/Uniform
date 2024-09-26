#ifndef __UNIFORM_CORELIBH
#define __UNIFORM_CORELIBH 1

#include "uniform/symboltable/shared.h"

#define UNIFORM_INTEGER_TYPE "Integer"

struct UniformCoreIntegerModuleStruct {
  void (*init)(UniformSymbolTable*);
};

extern struct UniformCoreIntegerModuleStruct UniformCoreIntegerModule;

#endif