#ifndef __UNIFORM_PREPROCESSORH
#define __UNIFORM_PREPROCESSORH 1

#include <dlfcn.h>
#include "uniform/scanner/shared.h"

#define UNIFORM_PREPROCESSOR_VERSION 100

typedef struct UniformPreprocessorStruct {
  void* scanner_library_handle;
  struct UniformScannerModuleStruct* scanner_module;
} UniformPreprocessor;

struct UniformPreprocessorModuleStruct {
  const signed int version;

  UniformPreprocessor* (*init)(const char*);
  void (*close)(UniformPreprocessor*);
};

extern struct UniformPreprocessorModuleStruct UniformPreprocessorModule;

#endif