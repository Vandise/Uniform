#include "uniform/compiler/shared.h"

// ============================
//          Forwards
// ============================

static UniformCompiler* init(UNIFORM_ARCH arch);

// ============================
//        Implementation
// ============================

static UniformCompiler* init(UNIFORM_ARCH arch) {
  UniformCompiler* compiler = malloc(sizeof(UniformCompiler));
  compiler->architecture = arch;

  switch(arch) {
    case UNIFORM_X_86: {
      UniformCompilerModule.initialized = 1;
      UniformCompilerModule.compile = UniformCompilerX86Module.compile;
      break;
    }
    case UNIFORM_X_86_64: {
      UniformCompilerModule.initialized = 1;
      UniformCompilerModule.compile = UniformCompilerX86_64Module.compile;
      break;
    }
    default:
      UniformCompilerModule.initialized = 0;
  }

  return compiler;
}

// ============================
//            Module
// ============================

struct UniformCompilerModuleStruct UniformCompilerModule = {
  .initialized = 0,
  .init = init
};