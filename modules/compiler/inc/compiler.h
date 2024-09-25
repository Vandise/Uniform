#ifndef __UNIFORM_COMPILERH
#define __UNIFORM_COMPILERH 1

#include <stdio.h>
#include <stdlib.h>
#include "uniform/ast/shared.h"

typedef enum {
  UNIFORM_UNKNOWN_ARCH,
  UNIFORM_X_86,
  UNIFORM_X_86_64
} UNIFORM_ARCH;

typedef struct UniformCompilerStruct {
  UNIFORM_ARCH architecture;
} UniformCompiler;

struct UniformCompilerModuleStruct {
  int initialized;
  UniformCompiler* (*init)(UNIFORM_ARCH);
  int (*compile)(UniformCompiler*, UniformAST*);
};

extern struct UniformCompilerModuleStruct UniformCompilerModule;

#endif