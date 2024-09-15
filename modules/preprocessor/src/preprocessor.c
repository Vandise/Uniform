#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static void init_uniform_chartable(UniformScanner* scanner);

static UniformPreprocessor* init(const char *library, int emit);
static void process(UniformPreprocessor *preprocessor, const char *file_name);
static void process_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner);
void register_macro(UniformPreprocessor* preprocessor, const char* macro, void(*action)(UniformPreprocessor*, UniformScanner*));
static void uniform_close(UniformPreprocessor* preprocessor);

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

  preprocessor->n_macro_size = 10;
  preprocessor->n_macro_used = 0;
  preprocessor->macros = malloc(sizeof(UniformMacro*) * 10);

  if (emit) {
    char time_buffer[20];
    time_t now = time(NULL);
    strftime(time_buffer, 20, "%Y%m%d%H", localtime(&now));
  
    strcpy(preprocessor->emit_file_name, ".tmp/");
    strcat(preprocessor->emit_file_name, time_buffer);
    strcat(preprocessor->emit_file_name, UNIFORM_FILE_EXTENSION);

    preprocessor->emit_file = fopen(preprocessor->emit_file_name, "a+");
  }

  return preprocessor;
}

void register_macro(UniformPreprocessor* preprocessor, const char* macro_name, void(*action)(UniformPreprocessor*, UniformScanner*)) {
  UniformLogger.log_info("Preprocessor::register_macro(name: %s)", macro_name);

  if (preprocessor->n_macro_used == preprocessor->n_macro_size) {
    preprocessor->n_macro_size = (preprocessor->n_macro_size * 3) / 2 + 8;
    preprocessor->macros = (UniformMacro*)(realloc(
      preprocessor->macros, preprocessor->n_macro_size * sizeof(UniformMacro)
    ));
  }

  UniformMacro macro;
  strcpy(macro.name, macro_name);
  macro.handle = action;

  preprocessor->macros[preprocessor->n_macro_used++] = macro;
}

static void process(UniformPreprocessor *preprocessor, const char *file_name) {
  UniformLogger.log_info("Preprocessor::process(file: %s, emit file: %s)", file_name, preprocessor->emit_file_name);

  UniformScanner *scanner = preprocessor->scanner_module->init(file_name);

  if (!scanner->errored) {
    init_uniform_chartable(scanner);
    do {
      preprocessor->scanner_module->get_token(scanner);
      if (scanner->current_token.code == T_MACRO) {
        process_macro(preprocessor, scanner);
      } else {
        // todo: execute macro if identifier and defined
        if (preprocessor->emit) {
          UniformTokenEmitterModule.emit_token(preprocessor, scanner);
        }
      }
    } while(scanner->current_token.code != T_END_OF_FILE);
  }

  free(scanner);
}

static void process_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner) {
  preprocessor->scanner_module->get_token(scanner);

  UniformLogger.log_info("Preprocessor::process_macro(macro: %s)", scanner->current_token.token_string);

  if (scanner->current_token.code != T_IDENTIFIER) {
    UniformLogger.log_fatal("Preprocessor::process_macro(error: invalid macro %s)", scanner->current_token.token_string);
    // todo: error
  }

  UniformMacro macro;
  int macro_found = 0;
  for (int i = 0; i < preprocessor->n_macro_used; i++) {
    if (strcmp(preprocessor->macros[i].name, scanner->current_token.token_string) == 0) {
      macro_found = 1;
      macro = preprocessor->macros[i];
      break;
    }
  }

  if (macro_found) {
    macro.handle(preprocessor, scanner);
    return;
  }

  UniformLogger.log_fatal("Preprocessor::process_macro(error: macro %s not found)", scanner->current_token.token_string);
}

static void uniform_close(UniformPreprocessor* preprocessor) {
  UniformLogger.log_info("Preprocessor::close");

  void *handle = preprocessor->scanner_library_handle;

  dlclose(handle);

  preprocessor->scanner_module = NULL;
  preprocessor->scanner_library_handle = NULL;

  if (preprocessor->macros != NULL) {
    free(preprocessor->macros);
  }

  if (preprocessor->emit) {
    fclose(preprocessor->emit_file);
  }

  free(preprocessor);
}

// ============================
//            Module
// ============================

struct UniformPreprocessorModuleStruct UniformPreprocessorModule = {
  .version = UNIFORM_PREPROCESSOR_VERSION,
  .log_level = UNIFORM_LOG_INFO,
  .init = init,
  .register_macro = register_macro,
  .process = process,
  .close = uniform_close,
};