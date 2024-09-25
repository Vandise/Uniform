#ifndef __UNIFORM_COMPILERX86H
#define __UNIFORM_COMPILERX86H 1

#include <stdio.h>
#include <stdlib.h>

#include "uniform/ast/shared.h"

struct UniformCompilerX86ModuleStruct {
  int (*compile)(UniformCompiler*, UniformAST*);
};

extern struct UniformCompilerX86ModuleStruct UniformCompilerX86Module;

#endif