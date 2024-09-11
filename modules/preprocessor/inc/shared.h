#ifndef __UNIFORM_PREPROCESSORH
#define __UNIFORM_PREPROCESSORH 1

#include <dlfcn.h>
#include "uniform/scanner/shared.h"

#define UNIFORM_PREPROCESSOR_VERSION 100
#define UNIFORM_PREPROCESSOR_PASSTHROUGH 0
#define UNIFORM_PREPROCESSOR_EMIT 1

typedef struct UniformPreprocessorStruct {
  int emit;
  void* scanner_library_handle;
  struct UniformScannerModuleStruct* scanner_module;
} UniformPreprocessor;

struct UniformPreprocessorModuleStruct {
  const signed int version;

  UniformPreprocessor* (*init)(const char*, int);
  void (*process)(UniformPreprocessor*, const char*);
  void (*close)(UniformPreprocessor*);
};

extern struct UniformPreprocessorModuleStruct UniformPreprocessorModule;

#endif