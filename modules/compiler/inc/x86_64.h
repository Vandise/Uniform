#ifndef __UNIFORM_COMPILERX86_64H
#define __UNIFORM_COMPILERX86_64H 1

#include <stdio.h>
#include <stdlib.h>

#include "uniform/ast/shared.h"

struct UniformCompilerX8664ModuleStruct {
  int (*compile)(UniformCompiler*, UniformAST*);
};

extern struct UniformCompilerX8664ModuleStruct UniformCompilerX86_64Module;

#endif