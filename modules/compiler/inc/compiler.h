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

typedef enum {
  UNIFORM_IOPT_NONE,
  UNIFORM_IOPT_RAX_SET,
  UNIFORM_IOPT_RCX_SET,
  UNIFORM_IOPT_RDX_SET,
} UNIFORM_IOPT_X86_64_FLAG;

typedef struct UniformCompilerStruct {
  UNIFORM_ARCH architecture;
  int iopt_flags;
} UniformCompiler;

struct UniformCompilerModuleStruct {
  int initialized;
  UniformCompiler* (*init)(UNIFORM_ARCH);
  int (*compile)(UniformCompiler*, UniformAST*);
};

extern struct UniformCompilerModuleStruct UniformCompilerModule;

#endif