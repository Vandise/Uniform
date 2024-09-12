#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static char* get_file_real_path(const char* s);
static void import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner);
static char* uniform_strndup(const char *s, size_t n);
static size_t uniform_strnlen(const char *src, size_t n);

// ============================
//        Implementation
// ============================

static size_t uniform_strnlen(const char *src, size_t n) {
  size_t len = 0;
  while (len < n && src[len]) { len++; }
  return len;
}


static char* uniform_strndup(const char *s, size_t n) {
  size_t len = uniform_strnlen(s, n);
  char *p = malloc(len + 1);
  if (p) {
    memcpy(p, s, len);
    p[len] = '\0';
  }
  return p;
}

static char* get_file_real_path(const char* s) {
  //
  // todo: expand ..'s
  //
  char *path_buffer = malloc(FILE_PATH_MAX);
  getcwd(path_buffer, FILE_PATH_MAX);

  char *last_slash = strrchr(s, '/');
  char *file_path = uniform_strndup(s, ((last_slash + 1) - s));

  strcat(path_buffer, file_path);

  free(file_path);

  return path_buffer;
}

static void import_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner) {
  UniformLogger.log_info("Macros::import_macro");

  char path_buffer[FILE_PATH_MAX];

  preprocessor->scanner_module->get_token(scanner); // T_OPEN_PAREN
  preprocessor->scanner_module->get_token(scanner); // T_STRING

  if (scanner->current_token.code != T_STRING) {
    UniformLogger.log_fatal("Macros::import_macro(error: invalid macro token. expected String)");
  }

  char* buffer = get_file_real_path(scanner->source_name);

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