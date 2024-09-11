#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static void init_uniform_chartable(UniformScanner* scanner);
static UniformPreprocessor* init(const char *library, int emit);
static void process(UniformPreprocessor *preprocessor, const char *file_name);
static void execute_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner);
static void close(UniformPreprocessor* preprocessor);

// ============================
//        Implementation
// ============================

static void init_uniform_chartable(UniformScanner* scanner) {
  int ch;

  for (ch = 0;   ch < 256;  ++ch) scanner->char_table[ch] = SPECIAL_CHAR_CODE;
  for (ch = '0'; ch <= '9'; ++ch) scanner->char_table[ch] = DIGIT_CHAR_CODE;
  for (ch = 'A'; ch <= 'Z'; ++ch) scanner->char_table[ch] = UPPERCASE_LETTER_CHAR_CODE;
  for (ch = 'a'; ch <= 'z'; ++ch) scanner->char_table[ch] = LETTER_CHAR_CODE;
  scanner->char_table['\''] = QUOTE_CHAR_CODE;
  scanner->char_table['"'] = QUOTE_CHAR_CODE;
  scanner->char_table[NEWLINE_CHAR] = NEWLINE_CHAR_CODE;
  scanner->char_table[CARRIAGE_RETURN_CHAR] = CARRIAGE_RETURN_CHAR_CODE;
  scanner->char_table['_'] = UNDERSCORE_CHAR_CODE;
  scanner->char_table[EOF_CHAR] = EOF_CHAR_CODE;
}


static UniformPreprocessor* init(const char *library, int emit) {
  UniformLogger.log_info("Preprocessor::init(library: %s, emit: %d)", library, emit);

  void *handle = dlopen(library, RTLD_LAZY);

  if (handle == NULL) {
    UniformLogger.log_fatal("Preprocessor::init(error: unable to open library %s)", library);
    return NULL;
  }

  UniformPreprocessor* preprocessor = malloc(sizeof(UniformPreprocessor));
  preprocessor->emit = emit;
  preprocessor->scanner_library_handle = handle;
  preprocessor->scanner_module = (struct UniformScannerModuleStruct*)dlsym(handle, "UniformScannerModule");
  preprocessor->scanner_module->set_log_level(UniformPreprocessorModule.log_level);

  return preprocessor;
}

static void process(UniformPreprocessor *preprocessor, const char *file_name) {
  UniformLogger.log_info("Preprocessor::process(file: %s)", file_name);

  UniformScanner *scanner = preprocessor->scanner_module->init(file_name);
  if (!scanner->errored) {
    init_uniform_chartable(scanner);
    do {
      preprocessor->scanner_module->get_token(scanner);
      if (scanner->current_token.code == T_MACRO) {
        execute_macro(preprocessor, scanner);
      } else {
        // todo: emit token based off of emit setting, or passthrough
      }
    } while(scanner->current_token.code != T_END_OF_FILE);
  }
  free(scanner);
}

static void execute_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner) {
  preprocessor->scanner_module->get_token(scanner);

  if (scanner->current_token.code != T_IDENTIFIER) {
    UniformLogger.log_fatal("Preprocessor::process(error: invalid macro %s)", scanner->current_token.token_string);
  }

  UniformLogger.log_info("Preprocessor::process(macro: %s)", scanner->current_token.token_string);
}

static void close(UniformPreprocessor* preprocessor) {
  UniformLogger.log_info("Preprocessor::close");

  void *handle = preprocessor->scanner_library_handle;

  dlclose(handle);

  preprocessor->scanner_module = NULL;
  preprocessor->scanner_library_handle = NULL;

  free(preprocessor);
}

// ============================
//            Module
// ============================

struct UniformPreprocessorModuleStruct UniformPreprocessorModule = {
  .version = UNIFORM_PREPROCESSOR_VERSION,
  .log_level = UNIFORM_LOG_INFO,
  .init = init,
  .process = process,
  .close = close
};