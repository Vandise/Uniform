#ifndef __UNIFORM_TYPESH
#define __UNIFORM_TYPESH 1

#include "uniform/logger/shared.h"
#include "uniform/symboltable/shared.h"

struct UniformParserTypeStruct {
  int (*assign_compatible)(UniformSymbolTableType*, UniformSymbolTableType*);
};

extern struct UniformParserTypeStruct UniformParserType;

#endif