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

typedef struct UniformMacroStruct {
  char *name;
  void (*handle)(UniformPreprocessor*, UniformScanner*);
} UniformMacro;



struct UniformPreprocessorModuleStruct {
  const signed int version;
  int log_level;

  UniformPreprocessor* (*init)(const char*, int);
  void (*process)(UniformPreprocessor*, const char*);
  void (*close)(UniformPreprocessor*);
};

struct UniformMacrosModuleStruct {
  void (*import_macro)(UniformPreprocessor*, UniformScanner*);
};

extern struct UniformPreprocessorModuleStruct UniformPreprocessorModule;
extern struct UniformMacrosModuleStruct UniformMacrosModule;

#endif