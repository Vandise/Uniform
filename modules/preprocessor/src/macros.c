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
  char buffer[FILE_PATH_MAX];
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
  UniformLogger.log_info("Macros::import_macro");

  char path_buffer[FILE_PATH_MAX];

  preprocessor->scanner_module->get_token(scanner); // T_OPEN_PAREN
  preprocessor->scanner_module->get_token(scanner); // T_STRING

  if (scanner->current_token.code != T_STRING) {
    UniformLogger.log_fatal("Macros::import_macro(error: invalid macro token. expected String)");
  }

  get_file_real_path(scanner->source_name, path_buffer);
  
  strcat(path_buffer, scanner->current_token.literal.value.string);
  strcat(path_buffer, UNIFORM_FILE_EXTENSION);

  UniformLogger.log_info("Macros::import_macro(file: %s)", path_buffer);

  preprocessor->scanner_module->get_token(scanner); // T_CLOSE_PAREN
  preprocessor->scanner_module->get_token(scanner); // T_NEWLINE

  UniformPreprocessorModule.process(preprocessor, path_buffer);
}

// ============================
//            Module
// ============================

struct UniformMacrosModuleStruct UniformMacrosModule = {
  .import_macro = import_macro
};