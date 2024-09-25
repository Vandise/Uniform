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
  UIOPT_NONE,

  UIOPT_RAX_SET,
  UIOPT_RCX_SET,
  UIOPT_RDX_SET,

  UIOPT_RAX_PUSHED,
  UIOPT_RCX_PUSHED,
  UIOPT_RDX_PUSHED,

  UIOPT_RAX_POPPED,
  UIOPT_RCX_POPPED,
  UIOPT_RDX_POPPED
} UNIFORM_IOPT_X86_64_FLAG;

typedef struct UniformCompilerStruct {
  UNIFORM_ARCH architecture;
  int iopt_flags;
  int node_index;
} UniformCompiler;

struct UniformCompilerModuleStruct {
  int initialized;
  UniformCompiler* (*init)(UNIFORM_ARCH);
  int (*compile)(UniformCompiler*, UniformAST*);
};

extern struct UniformCompilerModuleStruct UniformCompilerModule;

#endif