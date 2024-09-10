#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static UniformPreprocessor* init(const char *library);
static void close(UniformPreprocessor* preprocessor);

// ============================
//        Implementation
// ============================

static UniformPreprocessor* init(const char *library) {
  UniformLogger.log_info("Preprocessor::init(library: %s)", library);

  void *handle = dlopen(library, RTLD_LAZY);

  if (handle == NULL) {
    UniformLogger.log_fatal("Preprocessor::init(error: unable to open library %s)", library);
    return NULL;
  }

  UniformPreprocessor* preprocessor = malloc(sizeof(UniformPreprocessor));
  preprocessor->scanner_library_handle = handle;
  preprocessor->scanner_module = (struct UniformScannerModuleStruct*)dlsym(handle, "UniformScannerModule");

  return preprocessor;
}

static void close(UniformPreprocessor* preprocessor) {
  UniformLogger.log_info("Preprocessor::close");

  dlclose(preprocessor->scanner_library_handle);
  free(preprocessor);
}

// ============================
//            Module
// ============================

struct UniformPreprocessorModuleStruct UniformPreprocessorModule = {
  .version = UNIFORM_PREPROCESSOR_VERSION,
  .init = init,
  .close = close
};