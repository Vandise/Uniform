#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static int import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner, UniformScanner *initializer);

// ============================
//        Implementation
// ============================

static int import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner, UniformScanner *initializer) {
  UniformLogger.log_info("Macros::import_macro");

  char path_buffer[FILE_PATH_MAX];

  preprocessor->scanner_module->get_token(scanner); // T_OPEN_PAREN
  preprocessor->scanner_module->get_token(scanner); // T_STRING

  if (scanner->current_token.code != T_STRING) {
    UniformLogger.log_fatal("Macros::import_macro(error: invalid macro token. expected String)");
    return 1;
  }

  char* buffer = UniformFileUtil.get_file_path(scanner->source_name);

  strcpy(path_buffer, buffer);
  strcat(path_buffer, scanner->current_token.literal.value.string);
  strcat(path_buffer, UNIFORM_FILE_EXTENSION);

  UniformLogger.log_info("Macros::import_macro(file: %s)", path_buffer);

  preprocessor->scanner_module->get_token(scanner); // T_CLOSE_PAREN

  int errored = UniformPreprocessorModule.process(preprocessor, path_buffer, scanner);

  free(buffer);

  if (errored) { return 1; }

  preprocessor->scanner_module->get_token(scanner); // T_NEWLINE

  return 0;
}

// ============================
//            Module
// ============================

struct UniformMacrosModuleStruct UniformMacrosModule = {
  .import_macro = import_macro
};