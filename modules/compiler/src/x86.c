#include "uniform/compiler/shared.h"

// ============================
//          Forwards
// ============================

static int compile(UniformCompiler* compiler, UniformAST* tree);

// ============================
//        Implementation
// ============================

static int compile(UniformCompiler* compiler, UniformAST* tree) {
  return 0;
}

// ============================
//            Module
// ============================

struct UniformCompilerX86ModuleStruct UniformCompilerX86Module = {
  .compile = compile
};
