#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// ============================
//          Forwards
// ============================

int assign_compatible(UniformSymbolTableType*, UniformSymbolTableType*);

// ============================
//        Implementation
// ============================

int assign_compatible(UniformSymbolTableType* t1, UniformSymbolTableType* t2) {
  //
  // todo:
  //    this is a very basic assign type check that does not handle array forms
  //    or other mathematics such as int + float
  //
  if (t1 == t2) { return 1; }
  return 0;
}

// ============================
//            Module
// ============================

struct UniformParserTypeStruct UniformParserType = {
  .assign_compatible = assign_compatible
};