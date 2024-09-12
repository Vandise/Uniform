#ifndef __UNIFORM_PREPROCESSORH
#define __UNIFORM_PREPROCESSORH 1

#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "uniform/file/shared.h"
#include "uniform/scanner/shared.h"

#define UNIFORM_PREPROCESSOR_VERSION 100
#define UNIFORM_PREPROCESSOR_PASSTHROUGH 0
#define UNIFORM_PREPROCESSOR_EMIT 1
#define UNIFORM_FILE_EXTENSION ".u"
#define EMIT_TOKEN(s) fputs(s, preprocessor->emit_file);

typedef struct UniformPreprocessorStruct UniformPreprocessor;

typedef struct UniformMacroStruct {
  char name[64];
  void (*handle)(UniformPreprocessor*, UniformScanner*);
} UniformMacro;

typedef struct UniformPreprocessorStruct {
  int emit;
  void* scanner_library_handle;
  struct UniformScannerModuleStruct* scanner_module;

  FILE *emit_file;
  char emit_file_name[FILE_PATH_MAX];

  int n_macro_size;
  int n_macro_used;
  UniformMacro* macros;
} UniformPreprocessor;

struct UniformPreprocessorModuleStruct {
  const signed int version;
  int log_level;

  UniformPreprocessor* (*init)(const char*, int);
  void (*register_macro)(UniformPreprocessor*, const char*, void(*f)(UniformPreprocessor*, UniformScanner*));
  void (*process)(UniformPreprocessor*, const char*);
  void (*close)(UniformPreprocessor*);
};

struct UniformMacrosModuleStruct {
  void (*import_macro)(UniformPreprocessor*, UniformScanner*);
};

struct UniformTokenEmitterModuleStruct {
  void (*emit_token)(UniformPreprocessor*, UniformScanner*);
};

extern struct UniformPreprocessorModuleStruct UniformPreprocessorModule;
extern struct UniformMacrosModuleStruct UniformMacrosModule;
extern struct UniformTokenEmitterModuleStruct UniformTokenEmitterModule;

#endif