#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static int file_exists(const char* filename);
static int get_file_real_path(const char* s, char* buffer);
static void import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner);

// ============================
//        Implementation
// ============================

static int file_exists(const char* filename) {
  char buffer[strlen(filename) + 255];
  char* bufferp = buffer;

  realpath(filename, bufferp);

  FILE* file = fopen(bufferp, "r");
  if (file) {
    fclose(file);
    return 1;
  }
  return 0;
}

static int get_file_real_path(const char* s, char* buffer) {
  realpath(s, buffer);

  if (!file_exists(buffer)) {
    return 0;
  }

  int size = strlen(buffer);
  for (int i = size - 1; i > 0; i--) {
    if (buffer[i] == '/') {
      break;
    }
    buffer[i] = '\0';
  }

  return 1;
}

static void import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner) {
  
}

// ============================
//            Module
// ============================

struct UniformMacrosModuleStruct UniformMacrosModule = {
  .import_macro = import_macro
};