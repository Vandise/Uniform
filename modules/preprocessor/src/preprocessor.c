#include "uniform/preprocessor/shared.h"

// ============================
//          Forwards
// ============================

static void init_uniform_chartable(UniformScanner* scanner);
static unsigned long uniform_fhash(char *str);

static UniformPreprocessor* init(const char *library, int emit);
static int process(UniformPreprocessor *preprocessor, const char *file_name, UniformScanner *initializer);
static int process_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner, UniformScanner *initializer);
static void register_macro(UniformPreprocessor* preprocessor, const char* macro, int(*action)(UniformPreprocessor*, UniformScanner*, UniformScanner*));
static void uniform_close_file_streams(UniformPreprocessor* preprocessor);
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

static unsigned long uniform_fhash(char *str) {
  unsigned long hash = (rand() % (1000 + 1));
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash + (long)time(NULL);
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
  preprocessor->emit_file = NULL;
  preprocessor->errored = 0;
  preprocessor->scanner_library_handle = handle;

  preprocessor->scanner_module = (struct UniformScannerModuleStruct*)dlsym(handle, "UniformScannerModule");
  preprocessor->scanner_module->set_log_level(UniformPreprocessorModule.log_level);
  preprocessor->token_module = (struct UniformTokenModuleStruct*)dlsym(handle, "UniformTokenModule");

  preprocessor->token_array = preprocessor->token_module->init(10);

  preprocessor->n_macro_size = 10;
  preprocessor->n_macro_used = 0;
  preprocessor->macros = malloc(sizeof(UniformMacro*) * 10);

  if (emit) {
    char fn_buffer[45];

    time_t now = time(NULL);
    strftime(fn_buffer, 14, "%Y%m%d%H%M%S", localtime(&now));
    snprintf(fn_buffer, 20, "%s_%lu", fn_buffer, uniform_fhash(fn_buffer));

    strcpy(preprocessor->emit_file_name, ".tmp/");
    strcat(preprocessor->emit_file_name, fn_buffer);
    strcat(preprocessor->emit_file_name, UNIFORM_FILE_EXTENSION);

    preprocessor->emit_file = fopen(preprocessor->emit_file_name, "a+");
  }

  return preprocessor;
}

static void register_macro(UniformPreprocessor* preprocessor, const char* macro_name, int(*action)(UniformPreprocessor*, UniformScanner*, UniformScanner*)) {
  UniformLogger.log_info("Preprocessor::register_macro(name: %s, preprocessor: %p)", macro_name, preprocessor);

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

static int process(UniformPreprocessor *preprocessor, const char *file_name, UniformScanner *initializer) {
  UniformLogger.log_info(
    "Preprocessor::process(file: %s, emit file: %s / %s, initializer: %p)",
    file_name, preprocessor->emit_file_name,
    initializer
  );

  if (preprocessor->errored) { return 1; }

  UniformScanner *scanner = preprocessor->scanner_module->init(file_name);

  if (!scanner->errored) {
    init_uniform_chartable(scanner);

    do {
      preprocessor->scanner_module->get_token(scanner);

      if (scanner->current_token.code == T_MACRO) {
        preprocessor->errored = process_macro(preprocessor, scanner, initializer);
        if (preprocessor->errored) { break; }
      } else {
        // todo: execute macro if identifier and defined

        //
        // imports have an EOF signal, convert to newline for the parser
        // attach a EOF token after the preprocessor completes
        //
        if (scanner->current_token.code == T_END_OF_FILE) {
          UniformToken t = { .code = T_NEWLINE };
          preprocessor->token_module->commit_token(preprocessor->token_array, t);
        } else {
          preprocessor->token_module->commit_token(preprocessor->token_array, scanner->current_token);
        }

        if (preprocessor->emit) {
          UniformTokenEmitterModule.emit_token(preprocessor, scanner);
        }
      }
    } while(scanner->current_token.code != T_END_OF_FILE && !preprocessor->errored);

  } else {
    UniformScanner *es = initializer == NULL ? scanner : initializer;
    UniformErrorUtil.trace_error(UNIFORM_FILE_NOT_FOUND, es->source_name, es->line_number, es->buffer_offset, file_name);
    preprocessor->errored = 1;
  }

  free(scanner);

  return preprocessor->errored;
}

static int process_macro(UniformPreprocessor *preprocessor, UniformScanner *scanner, UniformScanner *initializer) {
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
    return macro.handle(preprocessor, scanner, initializer);
  }

  UniformLogger.log_fatal("Preprocessor::process_macro(error: macro %s not found)", scanner->current_token.token_string);

  return 1;
}

static void uniform_close(UniformPreprocessor* preprocessor) {
  UniformLogger.log_info("Preprocessor::close");

  preprocessor->token_module->clear(preprocessor->token_array);

  dlclose((void*)preprocessor->scanner_library_handle);

  preprocessor->scanner_module = NULL;
  preprocessor->scanner_library_handle = NULL;

  if (preprocessor->macros != NULL) {
    free(preprocessor->macros);
  }

  uniform_close_file_streams(preprocessor);

  free(preprocessor);
}

static void uniform_close_file_streams(UniformPreprocessor* preprocessor) {
  UniformLogger.log_info("Preprocessor::close_file_streams(file: %s [%p])", preprocessor->emit_file_name, preprocessor->emit_file);

  if(preprocessor->emit && preprocessor->emit_file != NULL) {
    fclose(preprocessor->emit_file);
    preprocessor->emit_file = NULL;
    //rename(preprocessor->lock_file_name, preprocessor->emit_file_name);
  }
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
  .close_file_streams = uniform_close_file_streams
};