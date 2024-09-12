#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static void import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner);

// ============================
//        Implementation
// ============================

static void import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner) {
  UniformLogger.log_info("Macros::import_macro");

  char path_buffer[FILE_PATH_MAX];

  preprocessor->scanner_module->get_token(scanner); // T_OPEN_PAREN
  preprocessor->scanner_module->get_token(scanner); // T_STRING

  if (scanner->current_token.code != T_STRING) {
    UniformLogger.log_fatal("Macros::import_macro(error: invalid macro token. expected String)");
  }

  char* buffer = UniformFileUtil.get_file_path(scanner->source_name);

  strcpy(path_buffer, buffer);
  strcat(path_buffer, scanner->current_token.literal.value.string);
  strcat(path_buffer, UNIFORM_FILE_EXTENSION);

  UniformLogger.log_info("Macros::import_macro(file: %s)", path_buffer);

  preprocessor->scanner_module->get_token(scanner); // T_CLOSE_PAREN
  preprocessor->scanner_module->get_token(scanner); // T_NEWLINE

  UniformPreprocessorModule.process(preprocessor, path_buffer);

  free(buffer);
}

// ============================
//            Module
// ============================

struct UniformMacrosModuleStruct UniformMacrosModule = {
  .import_macro = import_macro
};