#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static void init_uniform_chartable(UniformScanner* scanner);
static UniformPreprocessor* init(const char *library, int emit);
static void process(UniformPreprocessor *preprocessor, const char *file_name);
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

  return preprocessor;
}

static void process(UniformPreprocessor *preprocessor, const char *file_name) {
  UniformScanner *scanner = preprocessor->scanner_module->init(file_name);
  init_uniform_chartable(scanner);
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